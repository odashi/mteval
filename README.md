mteval Toolkit
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

You need following tools to build `mteval` implementations.
* `GCC 4.7` or later
* `Boost 1.49` or later
* `autotools`

You simply run below:

    $ cd /path/to/mteval
    $ autoreconf -i
    $ ./configure
    $ make
    $ (sudo) make install


Usage
-----

MTEval now have 3 types of evaluation algorithm:
* `mteval-corpus` - corpus-wise evaluation
* `mteval-sentence` - sentence-wise evaluation
* `mteval-pairwise` - pairwise bootstrap resampling

`mteval-corpus` and `mteval-sentence` requires 1 reference and 1 hypothesis corpus
to compute the goodness of the hypothesis.

`mteval-pairwise` requires 1 reference and 2 hypothesis corpus
to compute statistical significance of 1st hypothesis against 2nd hypothesis.

For example, we use a small example set described below:

    ref.tok:
    a b c d e
    a b c d e
    a b c d e

    hyp1.tok:
    a b c d e f
    a b c d e
    a c d e

    hyp2.tok:
    a b c d e
    a b c d e f g
    a c d e

Then, we type below example commands and get results:

    $ mteval-corpus -e BLEU RIBES -r ref.tok -h hyp1.tok
    BLEU=0.796902	RIBES=0.976918

    $ mteval-sentence -e BLEU RIBES -r ref.tok -h hyp1.tok
    BLEU=0.759836	RIBES=0.955443
    BLEU=1.000000	RIBES=1.000000
    BLEU=0.000000	RIBES=0.975310

    $ mteval-pairwise -i 1000 -s 100 -e BLEU RIBES -r ref.tok -h hyp1.tok hyp2.tok 
    BLEU: p=0.004000 (996/1000)	RIBES: p=0.009000 (991/1000)

(Note that results of `mteval-pairwise` changes randomly with a certain range)

Some evaluation metrics have parameters (e.g. maximum n-gram, or smoothing for BLEU).
You may set these parameters using `:param=value` notation:

    $ mteval-corpus -e BLEU:ngram=5:smooth=1 -r ref.tok -h hyp1.tok
    BLEU=0.805196

    $ mteval-corpus -e BLEU:smooth=1 -r ref.tok -h hyp1.tok
    BLEU=0.819619

Omitted parameters are assumed as default value.

If you need to obtain inner statistics of each evaluation metrics,
you can use `--output-stats` option for `mteval-corpus` and `mteval-sentence`:

    $ mteval-corpus --output-stats -e BLEU -r ref.tok -h hyp1.tok | sed 's/\t/\n/g'
    BLEU=0.796902
    BLEU:len:hyp=15
    BLEU:len:ref=15
    BLEU:ngram:1:hyp=15
    BLEU:ngram:1:match=14
    BLEU:ngram:2:hyp=12
    BLEU:ngram:2:match=10
    BLEU:ngram:3:hyp=9
    BLEU:ngram:3:match=7
    BLEU:ngram:4:hyp=6
    BLEU:ngram:4:match=4
    BLEU:samples=3

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
        * None
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

