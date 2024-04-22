Lossless Image Compression
--

CALIC Coding: Utilizes DPCM and CALIC prediction rules for encoding and decoding, measuring efficiency and accuracy through the average and standard deviation of absolute prediction errors and error histograms.

Huffman coding: Involves calculating pixel frequencies to build a Huffman tree for variable-length encoding, thereby compressing the image and enabling lossless reconstruction, with mean absolute error used to assess fidelity.

LZ77 Coding: Compresses and decompresses PGM images using sliding windows and decoding back, with efficiency and fidelity assessed via mean absolute error and histograms of offset and match lengths.

