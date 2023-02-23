import csv
import requests

# Number of days for the calculations
ndays = 7

def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    # 56 positions (states + other territories)
    nstates = 56
    # Number of days for the calculation of the average
    days = ndays * 2
    l = list(reversed(list(reader)))
    st = dict()

    for i in range(nstates):
        st.update({l[i]['state']: []})

    # First, adding the number of cases for the last (days + 1) days
    for i in range(days + 1):
        # 56 positions (states + other territories)
        for j in range(nstates):
            # add the number of cases to the list
            inx = j + i * nstates
            st[l[inx]['state']].append(int(l[inx]['cases']))

    for state in st:
        for j in range(days):
            st[state][j] = st[state][j] - st[state][j + 1]
        del(st[state][-1])

    # Return the dict
    return st

# Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for st in states:
        av = 0
        av2 = 0
        for d in range(ndays):
            av += new_cases[st][d]
            av2 += new_cases[st][d + ndays]
        av /= 7
        av2 /= 7
        chng = (av - av2) / av2
        percents = abs(round(chng * 100))
        if chng >= 0:
            print(f"{st} had a 7-day average of {round(av)} and an increase of {percents}%.")
        else:
            print(f"{st} had a 7-day average of {round(av)} and a decrease of {percents}%.")


main()
