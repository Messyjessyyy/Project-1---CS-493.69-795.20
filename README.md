# Connected Component Labeling CLI (`p2`)

## Usage

```bash
./p2 <input_binary_pgm> <output_labeled_pgm> [--thresh INT] [--ascii] [--4]
```

- `--thresh`: Threshold for binarizing non-binary inputs (default 0). Pixels > thresh become foreground.
- `--ascii`: Write ASCII PGM (P2). Default writes binary PGM (P5).
- `--4`: Use 4-connectivity (default is 8-connectivity).

Input must be PGM (`P2` or `P5`). Output is a labeled PGM where each connected object has a distinct gray level corresponding to its label.

## Example

Create a small P2 image with two objects and label it:

```bash
printf "P2\n6 5\n255\n0 0 0 0 0 0\n0 255 255 0 0 0\n0 255 255 0 255 0\n0 0 0 0 255 0\n0 0 0 0 0 0\n" > test_two.pgm
./p2 test_two.pgm out_ascii.pgm --thresh 128 --ascii
sed -n '1,20p' out_ascii.pgm
```

Expected output header and data (labels mapped to gray levels):

```text
P2
6 5
255
0 127 127 0 0 0
0 127 127 0 254 0
0 0 0 0 254 0
0 0 0 0 0 0
```

## Notes

- Implements standard two-pass connected-component labeling with union-find to resolve label equivalences.
- Supports both 8- and 4-connectivity.
- For binary inputs where pixels are 0/1 already, thresholding is skipped.
