# SSH using the C programming language

## How to run

**Do not forget to create a `target/` directory.**

### UNIX

Be sure to have the `libssh` dev package installed.\
[libssh-dev](https://packages.debian.org/bookworm/libssh-dev) on debian.\
Get the `libssh` library on Ubuntu/Debian using the following command:
```shell
apt-get install libssh-dev
```

**Run...**
- `make` or `make run` to build and run the program.
- `make build` to only build the program (built into the `target` directory).
- `make clean` to remove everything in the `target` directory.

### Windows

Should work in `wsl`.\
For that see the **UNIX** instructions.

## School Assignment

Write an application that connects to `test.rebex.net` on port `22` via SSH.

- User: demo
- Password: password

1. Print out the contents and permissions of the root directory.
1. Print out the contents of the `pub/example` directory.
1. Extra extensions:
    - visual representation (not implemented)
    - user input for `host`, `port`, `username` and password
