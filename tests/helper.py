import pytest
import tempfile
import os
import random
import string


def generate_data():
    symbols = string.ascii_uppercase + string.ascii_lowercase + string.digits
    size = random.randint(0, 100)
    data = ''.join(random.choice(symbols) for _ in range(size))
    return data


@pytest.fixture(scope='function')
def tempfile_path(request):
    with tempfile.NamedTemporaryFile(mode='w', delete=False) as file:
        path = file.name
        data = generate_data()
        file.write(data)

    def remove():
        os.remove(path)
    request.addfinalizer(remove)
    return path
