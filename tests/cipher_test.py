import pytest
from config import PATH
from helper import tempfile_path
import subprocess
import os
import filecmp
import shutil


class TestCipher(object):
    ext = '.x'

    def test_encrypt_file(self, PATH, tempfile_path):
        encrypted_path = tempfile_path + self.ext
        try:
            subprocess.check_call(
                [PATH['cryptor'], '-t', '-e', tempfile_path,
                 '-o', os.path.dirname(tempfile_path)])
            assert(filecmp.cmp(tempfile_path, encrypted_path, shallow=False)
                   is False)
        finally:
            os.remove(encrypted_path)

    def test_decrypt_file(self, PATH, tempfile_path):
        decrypted_path = tempfile_path + '.origin'
        shutil.copy(tempfile_path, decrypted_path)
        encrypted_path = tempfile_path + '.origin' + self.ext
        try:
            subprocess.check_call(
                [PATH['cryptor'], '-t', '-e', decrypted_path,
                 '-o', os.path.dirname(tempfile_path)])
            os.remove(decrypted_path)
            subprocess.check_call(
                [PATH['cryptor'], '-t', '-d', encrypted_path,
                 '-o', os.path.dirname(tempfile_path)])
            assert(filecmp.cmp(tempfile_path, decrypted_path, shallow=False)
                   is True)
        finally:
            os.remove(encrypted_path)
            os.remove(decrypted_path)
