import pytest
from config import PATH
from helper import file_was_wiped, tempfile_path, tempdir_path
import subprocess
import os
import filecmp
import shutil


class TestCryptor(object):
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
        assert(file_was_wiped(tempfile_path) is True)

    def test_wipe_file_and_rm(self, PATH, tempfile_path):
        subprocess.check_call(
            [PATH['cryptor'], '-w', tempfile_path])
        assert(os.path.exists(tempfile_path) is False)

    def test_wipe_directory(self, PATH, tempdir_path):
        subprocess.check_call(
            [PATH['cryptor'], '-t', '-w', tempdir_path])
        for dirpath, dirnames, filenames in os.walk(tempdir_path):
            for name in filenames:
                assert(file_was_wiped(os.path.join(dirpath, name)) is True)

    def test_wipe_directory_and_rm(self, PATH, tempdir_path):
        subprocess.check_call(
            [PATH['cryptor'], '-w', tempdir_path])
        assert(os.path.exists(tempdir_path) is False)
