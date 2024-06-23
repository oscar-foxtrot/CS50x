from flask import Flask, render_template, request
from cs50 import SQL


app = Flask(__name__)

db = SQL('sqlite:///colors.db')

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "GET":
        colors = db.execute('SELECT * FROM color')
        return render_template("index.html", colors=colors)
    else:
        colors = db.execute('SELECT * FROM color')
        color = request.form.get("color")
        if color in [color['name'] for color in colors]:
            return render_template("color.html", color=color)
        else:
            return "failure"
