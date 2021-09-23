# Blockchain - CLI

# Blockchain command line interpreter

## Installation


### Prerequisities
* OpenSSL packages 

```bash
sudo apt-get install libssl1.0.0
sudo apt-get install libssl-dev
```
* `llist.so` and `llist.h`
[https://github.com/holbertonschool/holbertonschool-blockchain/tree/master/llist](https://github.com/holbertonschool/holbertonschool-blockchain/tree/master/llist)

### Compilation
Navigate to `cli/`:
```bash
cd cli/
```
```bash
make cli
```

## Use
```bash
./cli
```
to star the cli


## Commands
### `wallet_load` \[\<path>\]

### `wallet_save` \[\<path>\]

### `send` \<amount> \<address>
  * \<amount> the number of coins to send
  * \<address> public key of the recipient
### `mine`
### `info`
### `load` \[\<path>\]
### `save` \[\<path>\]
