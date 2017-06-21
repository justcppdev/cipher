import pytest
from config import PATH
from helper import tempfile_path
import subprocess
import os
import filecmp
import shutil


class TestCryptor(object):
    ext = '.x'
    chunk_size = 256

    def test_encrypt_file(self, PATH, tempfile_path):
        encrypted_path = tempfile_path + self.ext
        try:
            subprocess.check_call(
                [PATH['cryptor'], '-e', tempfile_path,
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
                [PATH['cryptor'], '-e', decrypted_path,
                 '-o', os.path.dirname(tempfile_path)])
            os.remove(decrypted_path)
            subprocess.check_call(
                [PATH['cryptor'], '-d', encrypted_path,
                 '-o', os.path.dirname(tempfile_path)])
            assert(filecmp.cmp(tempfile_path, decrypted_path, shallow=False)
                   is True)
        finally:
            os.remove(encrypted_path)
            os.remove(decrypted_path)

    def test_encrypt_directory(self):
        pass

    def test_decrypt_directory(self):
        pass

    def test_encrypt_some_targets(self):
        pass

    def test_decrypt_some_targets(self):
        pass

    def test_wipe_file(self, PATH, tempfile_path):
        subprocess.check_call(
            [PATH['cryptor'], '-t', '-w', tempfile_path])
        with open(tempfile_path, 'rb') as file:
            for chunk in iter(lambda: file.read(self.chunk_size), b''):
                for c in chunk:
                    assert(c == 0)

    def test_wipe_directory(self):
        pass
