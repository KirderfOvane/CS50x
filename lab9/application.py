import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Pickup the user entry from req.body
        birthdayName = request.form.get("name")
        birthdayMonth = request.form.get("month")
        birthdayDay = request.form.get("day")

        # Validate user input
        if not birthdayName:
            birthdays = db.execute("SELECT * FROM birthdays;")
            return render_template("/index.html",message = "No name provided",birthdays=birthdays)
        if not birthdayMonth:
            birthdays = db.execute("SELECT * FROM birthdays;")
            return render_template("/index.html",message = "No month provided",birthdays=birthdays)
        if not birthdayDay:
            birthdays = db.execute("SELECT * FROM birthdays;")
            return render_template("/index.html",message = "No day provided",birthdays=birthdays)

        # Add the user's entry into the database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)",birthdayName, birthdayMonth, birthdayDay)

        return redirect("/")

    else:

        # Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays;")
        return render_template("index.html",birthdays=birthdays)

