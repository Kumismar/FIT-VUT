encryption = {
    "x08": "a",
    "x36": "b",
    "x54": "c",
    "x51": "d",
    "x48": "e",
    "x04": "f",
    "x16": "g",
    "x07": "h",
    "x31": "i",
    "x57": "j",
    "x28": "k",
    "x30": "l",
    "x41": "m",
    "x24": "n",
    "x13": "o",
    "x06": "p",
    "x53": "q",
    "x01": "r",
    "x52": "s",
    "x27": "t",
    "x38": "u",
    "x00": "v",
    "x58": "w",
    "x17": "x",
    "x14": "y",
    "x46": "z",
    "x20": "0",
    "x44": "1",
    "x34": "2",
    "x33": "3",
    "x40": "4",
    "x50": "5",
    "x39": "6",
    "x12": "7",
    "x21": "8",
    "x60": "9",
    "x47": "aa",
    "x23": "bb",
    "x45": "cc",
    "x55": "dd",
    "x59": "ee",
    "x37": "ff",
    "x15": "gg",
    "x49": "hh",
    "x42": "ii",
    "x11": "jj",
    "x18": "kk",
    "x10": "ll",
    "x03": "mm",
    "x29": "nn",
    "x19": "oo",
    "x56": "pp",
    "x43": "qq",
    "x26": "rr",
    "x32": "ss",
    "x22": "tt",
    "x05": "uu",
    "x02": "vv",
    "x09": "ww",
    "x35": "xx",
    "x61": "yy",
    "x25": "zz"
}

i = 0
all_messages = ""
with open("../cipher.txt") as f:
    for line in f:
        decrypted_message = ""
        for encrypted_char in line.replace(',', '').strip().split():
            decrypted_char = encryption[encrypted_char]
            decrypted_message += decrypted_char
        print(f"ZP{i}: {decrypted_message}")
        all_messages += decrypted_message + "\n"
        i += 1
        decrypted_message = ""

list_of_messages = []
for message in all_messages.split('\n'):
    list_of_messages.append(list(message))

messages_csv_format = ""
for message in list_of_messages:
    for char in message:
        messages_csv_format += char + ';'
    messages_csv_format += '\n'

with open("output.csv", 'w') as f:
    f.write(messages_csv_format)
