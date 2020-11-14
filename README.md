# WebClient
A web client that sends and receives HTTP messages.

## Installation (Linux)

### Optional Dependencies (XAMPP and Valgrind)
```shell
$ chmod 755 xampp-linux-*-installer.run
$ sudo ./xampp-linux-*-installer.run
$ sudo apt-get install valgrind
```

### Running Instructions
```shell
$ make
$ ./web_client URL
```

## Optional Tools

### Valgrind - For Memory Checking

#### Installation (Linux)

```shell
$ sudo apt-get install valgrind
```

#### Usage (Linux)

```shell
$ valgrind --leak-check=full ./web_client URL
```

### [XAMPP](https://apachefriends.org/index.html) - Web Server For Testing (An Apache Distribution)

#### Installation (Linux)

```shell
$ chmod 755 xampp-linux-*-installer.run
$ sudo ./xampp-linux-*-installer.run
```

#### Starting The Server

```shell
$ sudo /opt/lampp/lampp start
```

Valgrind's `--undef-value-errors=no` option disables warnings regarding undefined values (and also speeds up the memory checking process), however, suppressing warnings isn't a good practice.

**Notes:**  

