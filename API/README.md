# makeNTU API

This folder contains a small Flask-based API used by the makeNTU project to record and retrieve water intake and to control reminder settings for devices.

## Overview

The API stores user data in simple JSON files located in the same folder as `app.py`. Each user is represented by a file named `<token>.json` where `token` is the user identifier.

Endpoints are basic and use URL path parameters. There is no authentication beyond possession of the token file.

## Dependencies

-   Python 3.7+
-   Flask

You can install dependencies with:

```bash
python3 -m pip install -r requirements.txt
```

## Running the server

From the `API/` directory run:

```bash
export FLASK_APP=app.py
flask run --host=0.0.0.0 --port=5000
```

Or run directly with Python for development:

```bash
python3 app.py
```

Note: The included `app.py` creates an `admin_token.json` file on startup with sample fields used by the app. Ensure the process has write permission in the `API/` directory.

## Endpoints

All endpoints return HTTP 400 for malformed requests or when a user's token file does not exist.

-   GET /

    -   Description: Basic health/info endpoint.
    -   Response: Plain text message.

-   GET or POST /send_water/<token>/<water>
    -   Description: Record water intake for today for user `token`.
    -   Parameters:
        -   token: filename prefix for user JSON (no extension)
        -   water: numeric value (liters or milliliters depending on your app convention)
    -   Response: "ok" on success
    -   Example:

```bash
curl -X POST http://localhost:5000/send_water/mytoken/250
```

-   GET /remind/<token>
    -   Description: Query whether the server should remind the user now. If the user's `device` is set to 1 (computer), the endpoint will set `should_remind` to true and return remind=false (see source for exact logic).
    -   Response: JSON `{ "remind": true|false }`

```bash
curl http://localhost:5000/remind/mytoken
```

-   GET /get_water/<token>/<date>
    -   Description: Retrieve recorded water for a specific ISO date (YYYY-MM-DD).
    -   Response: JSON `{ "water": <value>, "remind": true|false }`

```bash
curl http://localhost:5000/get_water/mytoken/2023-04-30
```

-   GET or POST /do_not_disturb/<token>/<no_disturb>
    -   Description: Toggle user's do-not-disturb flag.
    -   Parameters: `no_disturb` must be `true` or `false` (string)
    -   Response: "ok"

```bash
curl -X POST http://localhost:5000/do_not_disturb/mytoken/true
```

-   GET or POST /change_device/<token>/<device>
    -   Description: Change user's device. `device` must be `computer` or `coaster`.
    -   Response: "ok"

```bash
curl -X POST http://localhost:5000/change_device/mytoken/computer
```

## Data format (user JSON)

Each user file (for example `mytoken.json`) is expected to contain a JSON object like:

```json
{
    "name": "admin",
    "do_not_disturb": false,
    "should_remind": false,
    "device": 0,
    "water": {
        "2023-04-30": 1200
    }
}
```

The app will overwrite the `admin_token.json` file at startup. Other user files should be created beforehand by whatever client or provisioning process you use.

## Notes and caveats

-   This API is intentionally minimal and uses filesystem JSON files for storage. It is not secure or suitable for production use without adding proper authentication, validation, and persistent storage.
-   The `do_not_disturb` endpoint currently writes the raw `no_disturb` string into the JSON which may store the string instead of a boolean; see `app.py` for details and consider fixing in code if needed.

## Next steps (suggested)

-   Add a small script to provision user JSON files.
-   Add input validation and better error messages.
-   Migrate storage to a lightweight DB (SQLite) if you need concurrency or durability.
