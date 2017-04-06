MTEval Toolkit
==============

`MTEval` - Collection of evaluation metrics and algorithms for machine translation.


About
-----

This software inplements some major machine translation evaluation metrics
and evaluation algorithms to be easily used.

If you used `MTEval` toolkit, please refer this software on your document with below link:
* https://github.com/odashi/mteval


Install
-------

`MTEval` depends on below libraries:
* `Boost 1.48` or later

And uses below toolkit to build libraries and executalbes:
* `CMake 3.1.0` or later

First, we make a root directory of the build tree:

    $ cd /path/to/mteval
    $ mkdir build
    $ cd build

Then we build the tool:

    $ cmake ..
    $ make -j <threads>

And optionally you can run unit tests:

    $ make test

All executables (see next section) are stored in the `build/bin` directory.


Usage
-----

MTEval currently have 3 executables:
* `mteval-corpus` - corpus-wise evaluation
* `mteval-sentence` - sentence-wise evaluation
* `mteval-pairwise` - pairwise bootstrap resampling

`mteval-corpus` and `mteval-sentence` requires 1 reference and 1 hypothesis corpus
to compute the goodness of the hypothesis.

`mteval-pairwise` requires 1 reference and 2 hypothesis corpus
to compute statistical significance of 1st hypothesis against 2nd hypothesis.

For example, we use a small example set described below:

    data/ref:
    a b c d e
    a b c d e
    a b c d e
    a b c d e

    data/hyp1:
    a b c d e
    a b c d e f
    a c d e
    a b x d e

    data/hyp2:
    a b c d e
    a b c d e f g
    a b c d e
    a b x d e

Then, we type below example commands and get results:

    $ cd /path/to/mteval
    $ build/bin/mteval-corpus -e BLEU RIBES -r data/ref -h data/hyp1
    BLEU=0.666113	RIBES=0.969124

    $ build/bin/mteval-sentence -e BLEU RIBES -r data/ref -h data/hyp1
    BLEU=1.000000	RIBES=1.000000
    BLEU=0.759836	RIBES=0.955443
    BLEU=0.000000	RIBES=0.975310
    BLEU=0.000000	RIBES=0.945742

    $ build/bin/mteval-pairwise -i 1000 -s 100 -e BLEU RIBES -r data/ref -h data/hyp{1,2}
    BLEU: p=0.986000 (14/1000) RIBES: p=0.089000 (911/1000)

(Note that results of `mteval-pairwise` changes randomly with a certain range)

Some evaluation metrics have parameters (e.g. maximum n-gram, or smoothing for BLEU).
You may set these parameters using `:param=value` notation:

    $ build/bin/mteval-corpus -e BLEU:ngram=5:smooth=1 -r data/ref -h data/hyp1
    BLEU=0.676009

    $ build/bin/mteval-corpus -e BLEU:smooth=1 -r data/ref -h data/hyp1
    BLEU=0.696471

Omitted parameters are assumed as default value.

If you need to obtain inner statistics of each evaluation metrics,
you can use `--output-stats` option for `mteval-corpus` and `mteval-sentence`:

    $ build/bin/mteval-corpus --output-stats -e BLEU -r data/ref -h data/hyp1 \
      | tr '\t' '\n'
    BLEU=0.666113
    BLEU:len:hyp=20
    BLEU:len:ref=20
    BLEU:ngram:1:hyp=20
    BLEU:ngram:1:match=18
    BLEU:ngram:2:hyp=16
    BLEU:ngram:2:match=12
    BLEU:ngram:3:hyp=12
    BLEU:ngram:3:match=7
    BLEU:ngram:4:hyp=8
    BLEU:ngram:4:match=4
    BLEU:samples=4

Type `mteval-*** --help` to see more information for each command.


List of Metrics
---------------

* **BLEU**
    * Identifier: `BLEU`
    * Parameters:
        * `ngram`: maximum n-gram length (default: `4`)
        * `smooth`: additional counts for >1-gram (default: `0`)
    * Statistics:
        * `len:hyp`: number of words in hypothesis sentences.
        * `len:ref`: number of words in reference sentences.
        * `ngram:%d:hyp`: number of n-grams in the hypothesis sentence.
        * `ngram:%d:match`: number of matched n-grams.
        * `samples`: number of evaluation samples.

* **NIST**
    * Identifier: `NIST`
    * Parameters:
        * `ngram`: maximum n-gram length (default: `5`)
    * Statistics:
        * `len:hyp`: number of words in hypothesis sentences.
        * `len:ref`: number of words in reference sentences.
        * `ngram:%d:hyp`: number of n-grams in the hypothesis sentence.
        * `ngram:%d:match`: cumulative weighted n-gram matches.
        * `samples`: number of evaluation samples.

* **RIBES**
    * Identifier: `RIBES`
    * Parameters:
        * `alpha`: weight of unigram precision (default: `0.25`)
        * `beta`: weight of brevity penalty (default: `0.1`)
    * Statistics:
        * `brevity`: cumulative brevity penalty for each evaluation sample.
        * `nkt`: cumulative Kendall's tau for each evaluation sample.
        * `prec`: cumulative unigram precision for each evaluation sample.
        * `samples`: number of evaluation samples.
        * `score`: cumulative RIBES score for each evaluation sample.

* **Word Error Rate**
    * Identifier: `WER`
    * Parameters:
        * `substitute`: weight of substituting ref/hyp words (default: `1.0`)
        * `insert`: weight of inserting a hyp word (default: `1.0`)
        * `delete`: weight of deleting a hyp word (default: `1.0`)
    * Statistics:
        * `distance`: cumulative Levenshtein distance for each evaluation sample.
        * `samples`: number of evaluation samples.
        * `score`: cumulative WER score for each evaluation sample.


Contributors
------------

* Yusuke Oda (@odashi) - Most coding

We are counting more contributions from you.


Contact
-------

If you find an issue, please contact Y.Oda
* @odashi_t on Twitter (faster than E-Mail)
* yus.takara (at) gmail.com

