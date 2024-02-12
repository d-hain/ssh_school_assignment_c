# SSH using PHP

## How to run

**Do not forget to create a `target/` directory.**

### UNIX

Be sure to have the `libssh` dev package installed.\
[libssh-dev](https://packages.debian.org/bookworm/libssh-dev) on debian.

**Run...**
- `make` or `make run` to build and run the program.
- `make build` to only build the program (built into the `target` directory).
- `make clean` to remove everything in the `target` directory.

### Windows

Maybe try using `wsl`.\
I have no idea on how to get the `libssh` library.


## School Assignment

Write an application that connects to `test.rebex.net` on port `22` via SSH.

- User: demo
- Password: password

1. Print out the contents and permissions of the root directory.
1. Print out the contents of the `pub/example` directory.
1. Extra extensions:
    - visual representation (not implemented)
    - user input for `host`, `port`, `username` and password
