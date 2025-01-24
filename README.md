# AHPP

## Environment

* System: Ubuntu 22.04.3 LTS
* Compiler: GCC 11.4.0

## Compile the code

```shell
./build.sh
```

## Running

1. Generate query nodes

```shell
cd data/
python3 genseed.py CiteSeer 1237 # python3 genseed.py dataset_name |U|
```

2. Run algorithm

```shell
./abppr -f ./data -g CiteSeer -a asrp -e 0.000001 --alpha 0.15 --beta 0.35
```

## Parameters

* -f `<dataset path>`
* -g `<dataset name>`
* -a `<algorithm name>`
* [-e `<epsilon> (default 0.000001)`]
* [--alpha `<alpha> (default 0.15)`]
* [--beta `<beta> (default 0.35)`]

## Instructions

* data/: datasets files.
* Ensure that the **Boost library** is installed on your system before running this project.
