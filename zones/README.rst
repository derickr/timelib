Building the timezone files
---------------------------

The script to use is ``makearchive.sh``, but it expects a few things:

- The directory contains a ``tzcode2014i.tar.gz`` file and a
  ``tzdata2014i.tar.gz`` file.
- You have a PECL SVN checkout in ``~/dev/php/pecl``
- You have a PHPDOC GIT checkout in ``~/dev/php/phpdoc``
- You can commit to PHP's GIT ``php-src`` repository.
- You can commit to PHP's GIT ``doc-base`` and ``doc-en`` repositories.
- You can commit to PHP's SVN repository.
- ``php`` is in your path.

Do not run this script, unless you're Derick Rethans.
