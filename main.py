from flask import Flask, render_template

app = Flask(__name__, static_url_path="", static_folder="public", template_folder="public")


@app.route("/")
def home():
    return render_template("home.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8888, debug=True)
