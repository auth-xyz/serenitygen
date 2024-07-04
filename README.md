<a id="readme-top"></a>

<!-- ABOUT THE PROJECT -->
[![Serenity][product-screenshot]]

------

## Project setup

#### Prerequisites
- [dpp](https://dpp.dev)
- [preferably a brain](https://en.wikipedia.org/wiki/Brain)
- [a discord.dev applicaiton](https://discord.dev)

------

#### Setting up

First of all, as always, clone the repository and cd into the cloned directory

```bash
git clone https://github.com/auth-xyz/serenitygen
cd serenitygen
```

Then, after that, make a dotenv (.env) file inside the `serenitygen` folder (on the root of the folder, to be specific)

```bash
touch .env
```

On the `.env`, add two entries, `DISCORD_TOKEN` and `GUILD_ID`.
Just like the following:

```bash
# .env
DISCORD_TOKEN=yourbotstoken
GUILD_ID=yourguildid
```

--------

#### Building

I gotta note, because of my laziness the executable will expect the `.env` file to be on the root,
So you must run the built binary from the `dist/` directory.

```bash
make
```

Yeah, that's literally it. You just run `make` and it *should* build.
And if there's any new update on the code and you'd like to build it again:

```bash
git pull # to make sure you have the most recent changes
make clean

make # to rebuild
```

------

#### Contributing

Feel free to contribute in any way you'd like, just make a PR.
If you have any questions on how to use the source code/edit the code dm me on Discord <actually.auth>




[product-screenshot]: https://auth-xyz.github.io/repositories/serenitygen/banner.png
