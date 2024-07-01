import os
import pytz

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    portfolio_data = db.execute(
        'SELECT * FROM portfolio WHERE user_id == ?',
        session["user_id"]
    )

    total_stock_price = 0
    for row in portfolio_data:
        row['stock_symbol'] = db.execute(
            'SELECT symbol FROM stocks WHERE id == ?', row['stock_id'])[0]['symbol']
        share_price = lookup(row['stock_symbol'])['price']
        shares_price = share_price * row['quantity']
        row['share_price'] = usd(share_price)
        row['shares_price'] = usd(shares_price)
        total_stock_price += shares_price

    balance = db.execute('SELECT cash FROM users WHERE id == ?', session["user_id"])[0]['cash']
    grand_total = balance + total_stock_price

    return render_template("index.html", portfolio_data=portfolio_data, balance=usd(balance), grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        data = lookup(request.form.get("symbol"))
        if not data:
            return apology("No such ticker exists", 400)
        try:
            if int(request.form.get("shares")) <= 0:
                return apology("Impossible to open a short position."
                               "Enter a positive number of shares you would like to buy", 400)
        except ValueError:
            return apology("Please, enter a whole number of shares", 400)

        # Write off the amount of money needed to buy shares
        cash = db.execute('SELECT cash FROM users WHERE id == ?', session["user_id"])[0]['cash']
        sum = float(data['price']) * int(request.form.get("shares"))
        cash -= sum

        # Not enough money
        if cash < 0:
            return apology("Insufficient funds", 403)

        db.execute('UPDATE users SET cash = ? WHERE id == ?', cash, session["user_id"])

        try:
            stock_id = db.execute('SELECT id FROM stocks WHERE symbol == ?',
                                  request.form.get('symbol'))[0]['id']
        except IndexError:
            db.execute('INSERT INTO stocks (symbol) VALUES (?)', request.form.get('symbol'))
            stock_id = db.execute('SELECT id FROM stocks WHERE symbol == ?',
                                  request.form.get('symbol'))[0]['id']

        # Insert the new transaction
        # Notice how all times are in US/EASTERN timezone
        db.execute('INSERT INTO transactions (user_id, sum, shares, symbol_id, timestamp) VALUES (?, ?, ?, ?, ?)',
                   session["user_id"],
                   -sum,
                   int(request.form.get("shares")),
                   stock_id,
                   datetime.now(pytz.timezone("US/Eastern")).strftime('%Y-%m-%d %H:%M:%S')
                   )

        # Make the change to portfolio
        current_stock = db.execute(
            'SELECT * FROM portfolio WHERE user_id == ? AND stock_id == ?', session["user_id"], stock_id)
        if not current_stock:
            db.execute('INSERT INTO portfolio (user_id, stock_id, quantity) VALUES (?, ?, ?)',
                       session["user_id"],
                       stock_id,
                       int(request.form.get("shares"))
                       )
        else:
            current_stock = current_stock[0]['quantity']
            current_stock += int(request.form.get("shares"))
            db.execute('UPDATE portfolio SET quantity = ? WHERE stock_id == ?',
                       current_stock, stock_id)

        flash(
            f"Successfully bought {request.form.get('shares')} {request.form.get('symbol')} shares")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute('SELECT * FROM transactions WHERE user_id == ?', session["user_id"])
    for row in transactions:
        row['stock_symbol'] = db.execute(
            'SELECT symbol FROM stocks WHERE id == ?', row['symbol_id'])[0]['symbol']
        share_price = -row['sum'] / row['shares']
        row['share_price'] = usd(share_price)
        row['shares_price'] = '-' + usd(-row['sum']) if row['sum'] < 0 else usd(row['sum'])
    return render_template("history.html", transactions=reversed(transactions))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Forget any user_id
        session.clear()

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        data = lookup(request.form.get("symbol"))
        if data:
            return render_template(
                "quoted.html",
                symbol=data["symbol"],
                price=usd(data["price"])
            )
        else:
            return apology(f"There's no \"{request.form.get("symbol")}\" ticker", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Server-side check of the repeated password being the same as the password
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("Passwords don't match", 400)

        # Try to insert a new row
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)",
                request.form.get("username"),
                generate_password_hash(request.form.get("password"))
            )
        except ValueError:
            return apology("username has already been taken", 400)

        # Redirect user to login page, after successfully registering
        flash("Registered successfully!")
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        try:
            stock_id = db.execute('SELECT id FROM stocks WHERE symbol == ?',
                                  request.form.get('symbol'))[0]['id']
        except IndexError:
            return apology(f"There are no shares with ticker {symbol} in your portfolio", 403)

        current_stock = db.execute(
            'SELECT * FROM portfolio WHERE user_id == ? AND stock_id == ?', session["user_id"], stock_id)[0]['quantity']

        if int(request.form.get("shares")) <= 0:
            return apology("Enter a positive number of shares you would like to sell", 400)
        elif current_stock - int(request.form.get("shares")) < 0:
            return apology("You don't have this many shares", 400)

        data = lookup(request.form.get("symbol"))
        # Increase amount of cash on account correspondingly to sold shares
        cash = db.execute('SELECT cash FROM users WHERE id == ?', session["user_id"])[0]['cash']
        sum = float(data['price']) * int(request.form.get("shares"))
        cash += sum
        db.execute('UPDATE users SET cash = ? WHERE id == ?', cash, session["user_id"])

        # Insert the new transaction
        # Notice how all times are in US/EASTERN timezone
        db.execute('INSERT INTO transactions (user_id, sum, shares, symbol_id, timestamp) VALUES (?, ?, ?, ?, ?)',
                   session["user_id"],
                   sum,
                   -int(request.form.get("shares")),
                   stock_id,
                   datetime.now(pytz.timezone("US/Eastern")).strftime('%Y-%m-%d %H:%M:%S')
                   )

        # Make the change to portfolio
        current_stock -= int(request.form.get("shares"))

        if current_stock != 0:
            db.execute('UPDATE portfolio SET quantity = ? WHERE stock_id == ?',
                       current_stock, stock_id)
        else:
            db.execute('DELETE FROM portfolio WHERE stock_id == ?', stock_id)

        flash(f"Successfully sold {request.form.get('shares')} {request.form.get('symbol')} shares")
        return redirect("/")
    else:
        portfolio_data = db.execute(
            'SELECT stock_id FROM portfolio WHERE user_id == ?',
            session["user_id"]
        )

        for row in portfolio_data:
            row['stock_symbol'] = db.execute(
                'SELECT symbol FROM stocks WHERE id == ?', row['stock_id'])[0]['symbol']

        return render_template("sell.html", tickers=portfolio_data)
