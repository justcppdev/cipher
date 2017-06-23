import pytest
import tempfile
import os
import random
import string
import shutil


def generate_data():
    symbols = string.ascii_uppercase + string.ascii_lowercase + string.digits
    size = random.randint(1, 100)
    data = ''.join(random.choice(symbols) for _ in range(size))
    return data


def generate_file(dir=None):
    with tempfile.NamedTemporaryFile(mode='w', dir=dir, delete=False) as file:
        path = file.name
        data = generate_data()
        file.write(data)

    return path


@pytest.fixture(scope='function')
def tempfile_path(request):
    path = generate_file()

    def remove():
        if (os.path.exists(path)):
            os.remove(path)

    request.addfinalizer(remove)
    return path


def generate_directory(dir=None, count=100):
    path = tempfile.mkdtemp(dir=dir)
    i = random.randint(1, count)
    count -= i

    while i > 0:
        generate_file(dir=path)
        i -= 1

    while count > 0:
        i = int(count / 2)
        if i != 0:
            generate_directory(dir=path, count=i)
            count -= i
        else:
            generate_directory(dir=path, count=count)
            count -= count

    return path


@pytest.fixture(scope='function')
def tempdir_path(request):
    path = generate_directory()

    def remove():
        shutil.rmtree(path, ignore_errors=True)

    request.addfinalizer(remove)
    return path


def file_was_wiped(path):
    with open(path, 'rb') as file:
        for chunk in iter(lambda: file.read(256), b''):
            for c in chunk:
                if c != 0:
                    return False
    return True
