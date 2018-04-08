""" setup for flatwhite
"""

import sys
from distutils.command.build_ext import build_ext 
from setuptools import setup, Extension
from setuptools.command.test import test as Test

class PyTest(Test):
    """ pytest class
    """
    def finalize_options(self):
        Test.finalize_options(self)
        self.test_args = []
        self.test_suite = True

    def run_tests(self):
        import pytest
        errcode = pytest.main(self.test_args)
        sys.exit(errcode)

setup(
    name='flatwhite',
    version='0.0.1',
    license='MIT License',
    author='a1trl9',
    tests_require=['pytest'],
    install_requires=[],
    packages=['flatwhite'],
    cmdclass={'test': PyTest},
    author_email='adavindes@gmail.com',
    description='ED Algorithm',
    include_package_data=True,
    platforms='any',
    # test_suite='',
    classifiers=[
        'Programming Language :: Python',
        'Development Status :: 0 - Alpha',
        'Natural Language :: English',
        'License :: MIT License',
        'Operating System :: OS Independent',
        'Topic :: Global/Local Edit Distance'
    ],
    extras_require={
        'testing': ['pytest']
        },
    ext_modules=[
        Extension('flatwhite.cflatwhite',
            ['cflatwhite/dr.c',
             'cflatwhite/ged.c',
             'cflatwhite/led.c',
             'cflatwhite/tf_idf.c',
             'cflatwhite/ngram.c',
             'cflatwhite/hashmap.c',
            ]
        )
    ]
    )