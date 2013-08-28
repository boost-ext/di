#!/usr/bin/env python

import os
import sys
import unittest
sys.path.append('../../scripts')
import di_ctor_traits

def read_file(path):
    with open(path, 'r') as content_file:
        content = content_file.read()
    return content

class TestDICtorTraits(unittest.TestCase):
    output_file = 'di_ctor_traits.hpp'

    def setUp(self):
        os.path.exists(self.output_file) and os.remove(self.output_file)

    def tearDown(self):
        os.path.exists(self.output_file) and os.remove(self.output_file)

    def test_di_ctor_traits_args_type_error(self):
        with self.assertRaises(TypeError):
            di_ctor_traits.main()

    def test_di_ctor_traits_args_fail_none(self):
        with self.assertRaises(SystemExit):
            di_ctor_traits.main(['./di_ctor_traits.py'])

    def test_di_ctor_traits_args_fail_file_not_found(self):
        self.assertEqual(0, di_ctor_traits.main(['../di_ctor_traits.py', 'not_found.hpp']))

    def test_di_ctor_traits_no_output(self):
        self.assertEqual(0, di_ctor_traits.main(['../di_ctor_traits.py', '-o', self.output_file, 'given/default_ctor.hpp']))
        self.assertFalse(os.path.exists(self.output_file))

    def test_di_ctor_traits(self):
        self.assertEqual(0, di_ctor_traits.main(['../di_ctor_traits.py', '-o', self.output_file, '-l', 'n', 'given/ctors.hpp']))
        self.assertEqual(read_file(self.output_file), read_file('then/ctors.hpp'))

if __name__ == '__main__':
    unittest.main()

