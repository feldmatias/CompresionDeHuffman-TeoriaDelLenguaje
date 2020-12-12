import string

READ_FILE = "long_text.txt"
STORE_FILE = "char_frequencies.txt"


def char_frequencies():
    chars = {
        char: 0 for char in string.printable
    }

    with open(READ_FILE, 'r') as f:
        for char in f.read():
            if char in chars:
                chars[char] += 1

    return chars


def store_frequencies(char_frequencies):
    with open(STORE_FILE, 'w') as f:
        keys = [char for char in string.printable]
        f.write(str(keys))

        f.write('\n')

        values = [char_frequencies[char] for char in string.printable]
        f.write(str(values))


if __name__ == "__main__":
    frequencies = char_frequencies()
    store_frequencies(frequencies)
