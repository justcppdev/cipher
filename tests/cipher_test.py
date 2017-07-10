import pytest
from config import PATH
from helper import tempfile_path, tempdir_path, dirs_are_eq
import subprocess
import os
import filecmp
import shutil


class TestCipher(object):
    ext = '.x'

    def test_encrypt_file(self, PATH, tempfile_path):
        output_dir = tempfile_path + '_output_dir'
        try:
            subprocess.check_call(
                [PATH['cipher'], '-t', '-e', tempfile_path,
                 '-o', output_dir])
            assert(filecmp.cmp(tempfile_path, os.path.join(
                    output_dir, os.path.basename(tempfile_path) + self.ext),
                               shallow=False) is False)
        finally:
            shutil.rmtree(output_dir, ignore_errors=True)

    def test_decrypt_file(self, PATH, tempfile_path):
        output_dir = tempfile_path + '_output_dir'
        try:
            subprocess.check_call(
                [PATH['cipher'], '-t', '-e', tempfile_path,
                 '-o', output_dir])
            subprocess.check_call(
                [PATH['cipher'], '-t', '-d', os.path.join(
                    output_dir, os.path.basename(tempfile_path) + self.ext),
                 '-o', output_dir])
            assert(filecmp.cmp(tempfile_path, os.path.join(
                    output_dir, os.path.basename(tempfile_path)),
                               shallow=False) is True)
        finally:
            shutil.rmtree(output_dir, ignore_errors=True)

    def test_encrypt_directory(self, PATH, tempdir_path):
        output_dir = tempdir_path + '_output_dir'
        try:
            subprocess.check_call(
                [PATH['cipher'], '-t', '-e', tempdir_path,
                 '-o', output_dir])
            assert(dirs_are_eq(tempdir_path, os.path.join(
                    output_dir, os.path.basename(tempdir_path))) is False)
        finally:
            shutil.rmtree(output_dir, ignore_errors=True)

    def test_decrypt_directory(self, PATH, tempdir_path):
        output_dir_1 = tempdir_path + '_output_dir_1'
        output_dir_2 = tempdir_path + '_output_dir_2'
        try:
            subprocess.check_call(
                [PATH['cipher'], '-t', '-e', tempdir_path,
                 '-o', output_dir_1])
            subprocess.check_call(
                [PATH['cipher'], '-t', '-d', os.path.join(
                    output_dir_1, os.path.basename(tempdir_path)),
                 '-o', output_dir_2])
            assert(dirs_are_eq(tempdir_path, os.path.join(
                    output_dir_2, os.path.basename(tempdir_path))) is True)
        finally:
            shutil.rmtree(output_dir_1, ignore_errors=True)
            shutil.rmtree(output_dir_2, ignore_errors=True)
