import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session,redirect
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Odds are you’ll want to execute multiple SELECTs.
    # Depending on how you implement your table(s), you might find GROUP BY HAVING SUM and/or WHERE of interest.
    cash = db.execute("SELECT cash FROM users WHERE id = ?",session['user_id'])
    stocks = db.execute("SELECT symbol,price,purchased_at,share_number,name,total FROM stocks WHERE userId = ?",session['user_id'])

    # Odds are you’ll want to call lookup for each stock.
    return render_template("index.html",cash=usd(cash[0]['cash']),stocks=stocks,usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Require that a user input a stock’s symbol, implemented as a text field whose name is symbol.
    if request.method == "GET":
       return render_template("buy.html")
    else:

        # Validate symbol
        symbol = request.form.get("symbol")
        if not symbol or symbol == None:
            return apology("You need to provide a valid symbol")
        stock = lookup(symbol)
        if not stock or None:
            return apology("Stock does not exist, please provide a valid symbol")

        # Validate shares
        shareInput = request.form.get("shares")
        if not shareInput:
            return apology("You need to provide number of shares")
        try:
            shareNumber = int(shareInput)
        except ValueError:
            return apology("You need to provide an integer number")
        if shareNumber < 0:
            return apology("You need to provide an positive number")


        # check that user cash is enough
        userCash = db.execute("SELECT cash FROM users WHERE id=?",session['user_id'])
        if userCash[0]['cash'] < shareNumber * stock['price']:
            return apology("You do not have enough cash to make this purchase")

        # create total sum of shares and price
        total = usd(stock['price'] * shareNumber)

        # make withdrawal
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash[0]['cash'] - shareNumber * stock['price'], session['user_id'])

        # persist purchase
        db.execute("INSERT INTO stocks (userId, symbol, price,name,share_number,total) VALUES(?,?,?,?,?,?)",session['user_id'], stock['symbol'],stock['price'],stock['name'],shareNumber,total)

        # persist record of transaction
        db.execute("INSERT INTO transactions (userId,symbol,price,share_number,total) VALUES(?,?,?,?,?)",session['user_id'], stock['symbol'], stock['price'],int(shareNumber),total)

        return redirect('/')

    # Render an apology if the input is blank or the symbol does not exist (as per the return value of lookup).
    # Require that a user input a number of shares, implemented as a text field whose name is shares.
    # Render an apology if the input is not a positive integer.
    # Submit the user’s input via POST to /buy.
    # Odds are you’ll want to call lookup to look up a stock’s current price.
    # Odds are you’ll want to SELECT how much cash the user currently has in users.
    # Add one or more new tables to finance.db via which to keep track of the purchase. Store enough information so that you know who bought what at what price and when.
    # Use appropriate SQLite types.
    # Define UNIQUE indexes on any fields that should be unique.
    # Define (non-UNIQUE) indexes on any fields via which you will search (as via SELECT with WHERE).
    # Render an apology, without completing a purchase, if the user cannot afford the number of shares at the current price.
    # When a purchase is complete, redirect the user back to the index page.
    # You don’t need to worry about race conditions (or use transactions).



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userTransactions = db.execute("SELECT symbol,price,share_number,total,created_at FROM transactions WHERE userId = ?",session['user_id'])
    # For each row, make clear whether a stock was bought or sold and include the stock’s symbol, the (purchase or sale) price, the number of shares bought or sold, and the date and time at which the transaction occurred.
    # You might need to alter the table you created for buy or supplement it with an additional table. Try to minimize redundancies.
    return render_template("history.html",transactions = userTransactions,usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    # Require that a user input a stock’s symbol, implemented as a text field whose name is symbol.
    stockQuote = {}
    if request.method == "GET":
        return render_template("quote.html")
    else:
        quote = request.form.get("symbol")

        if not quote or quote == None:
            return apology("You need  to fill in a symbol")
        stockQuote =  lookup(quote)
        if stockQuote == None:
            return apology("You need to provide a valid symbol")
        return render_template("quote.html",quote = stockQuote,usd=usd)
    # Submit the user’s input via POST to /quote.
    # Odds are you’ll want to create two new templates (e.g., quote.html and quoted.html).
    # When a user visits /quote via GET, render one of those templates, inside of which should be an HTML form that submits to /quote via POST.

    # In response to a POST, quote can render that second template, embedding within it one or more values from lookup.



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # pickup req.body / form inputs
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # "frontend" validation
        if not username:
            return apology("You forgot to provide a username")
        if not password:
            return apology("You forgot to provide a password")
        if not confirmation:
            return apology("You forgot to provide a confirmation of your password")
        if password != confirmation:
            return apology("Passwords do not match")

        # hash password
        hashedPassword = generate_password_hash(password,method='pbkdf2:sha256', salt_length=8)

        user = db.execute("SELECT username FROM users WHERE username=(?)",username)
        if user:
            return apology("Username already exist")
        try:
            db.execute("INSERT INTO users (username,hash) VALUES (?,?)",username,hashedPassword)
        except ValueError:
            return apology("username already taken")

        return redirect("/login")
    else:
        return render_template("register.html")
    # Require that a user input a username, implemented as a text field whose name is username
    # Render an apology if the user’s input is blank or the username already exists.
    # Require that a user input a password, implemented as a text field whose name is password, and then that same password again, implemented as a text field whose name is confirmation
    # Render an apology if either input is blank or the passwords do not match.
    # Submit the user’s input via POST to /register.
    # INSERT the new user into users, storing a hash of the user’s password, not the password itself.
    # Hash the user’s password with generate_password_hash Odds are you’ll want to create a new template (e.g., register.html) that’s quite similar to login.html.
    # Once the user is registered, you may either automatically log in the user or bring the user to a page where they can log in themselves.


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Require that a user input a stock’s symbol, implemented as a select menu whose name is symbol.
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please provide a valid symbol")

        # validate shares input
        shareInput = request.form.get("shares")
        if not shareInput:
            return apology("You need to provide number of shares")
        try:
            shares = int(shareInput)
        except ValueError:
            return apology("You need to provide an integer number")
        if shares < 0:
            return apology("You need to provide an positive number")


        # selects all rows of user stocks bought with that symbol and min. share_number. Can be multiple!, which is a problem.
        stockInfo = db.execute("SELECT id,symbol,share_number,price FROM stocks WHERE userId = ? AND symbol = ? AND share_number >= ?",session["user_id"],symbol,shares)

        if not stockInfo:
            return apology("No stock could be found matching those criterias")
        # calculate sale total sum
        stockFromIEX = lookup(symbol)
        stockPrice = stockFromIEX['price']
        sellSum = int(shares) * stockPrice

        # update user stock possession
        # if all shares is requested to be sold , delete the row from db
        if shares == stockInfo[0]['share_number']:
            #delete in db
            db.execute("DELETE FROM stocks WHERE id = ?",stockInfo[0]['id'])
        else:
            #update row in db
            db.execute("UPDATE stocks SET share_number = ?, total = ? WHERE id = ?",(stockInfo[0]['share_number'] - int(shares)),usd((stockInfo[0]['share_number'] - int(shares)) * stockInfo[0]['price']), stockInfo[0]['id'])

        # persist record of transaction
        db.execute("INSERT INTO transactions (userId,symbol,price,share_number,total) VALUES(?,?,?,?,?)",session['user_id'], stockInfo[0]['symbol'], stockInfo[0]['price'],int(shares) *-1,(int(shares) *-1) * stockInfo[0]['price'])

        # update user cash
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        newCash = currentCash[0]['cash'] + sellSum
        db.execute("UPDATE users SET cash = ? WHERE id = ?",newCash, session["user_id"])

        return redirect("/")
    else:
        # get all users stocks
        symbols = db.execute("SELECT symbol FROM stocks WHERE userId = ?",session['user_id'])
        return render_template("sell.html",symbols = symbols)
    # Render an apology if the user fails to select a stock or if (somehow, once submitted) the user does not own any shares of that stock.
    # Require that a user input a number of shares, implemented as a text field whose name is shares.
    # Render an apology if the input is not a positive integer or if the user does not own that many shares of the stock.
    # Submit the user’s input via POST to /sell.
    # When a sale is complete, redirect the user back to the index page.
    # You don’t need to worry about race conditions (or use transactions).


@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def updatePassword():
    # Allow users to change their passwords.

    # variable that helps frontend show password change text
    changed = False
    if request.method == "POST":
        newPassword = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if newPassword != confirmation:
            return apology("Passwords do not match")

        # Hash password
        hashedPassword = generate_password_hash(newPassword,method='pbkdf2:sha256', salt_length=8)

        test = db.execute("UPDATE users SET hash = ? WHERE id = ?", hashedPassword, session['user_id'])
        changed = True
        print(test)

    return render_template("userprofile.html",changed = changed)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
