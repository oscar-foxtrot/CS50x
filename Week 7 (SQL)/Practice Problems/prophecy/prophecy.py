import csv
import sqlite3

conn = sqlite3.connect("roster.db")
cursor = conn.cursor()

# Create the new schema
with open('create.sql', 'r') as sql_file:
    sql_commands = sql_file.read()
    cursor.executescript(sql_commands)

'''
for row in conn.execute("SELECT * FROM students"):
   print(row)
'''

# Fill the newly created tables
with open('script.sql', 'r') as sql_file:
    sql_commands = sql_file.read()
    cursor.executescript(sql_commands)

conn.commit()
conn.close()
