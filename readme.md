# Sanddollar

Sanddollar is a script to allow running C like code in script like fashion.
add a shebang line to the top of the file that points to sanddollar.py and set the permissions of the file to execute and you can run the file as with standard script languages.

For ease of you I recommend setting up a symbolic link to the script in /usr/bin

e.g.

```bash
cd ~
git clone https://github.com/buxtonpaul/sanddollar.git
sudo ln -s  ~/sanddollar/sanddollar.py /usr/bin/sanddollar
```

You should now be able to run the sample
```
cd sanddollar
./test.run
```


----
## Status
This is a quick rough implementation.
The programs are limited to single files.
Currently only plain C is supported.
There is no caching of compiled scripts
No way to validate a script
