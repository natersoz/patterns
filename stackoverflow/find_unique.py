''' Someone's homework assignment on Stackoverflow '''

import argparse

# When a search for a match comes up empty.
NOT_FOUND = -1

# Print debugging information, increasing values are more verbose.
PRINT_DEBUG = 0

def data_string_to_bytes(data_string):
    '''
    Convert packed string of nybble data to bytes.
    There is expected to be no whitespace intermingled within.
    The number of nybbles must be even.
    '''
    if len(data_string) % 2 != 0:
        raise ValueError('Attempting to parse byte string with odd legnth {}'
                         .format(len(data_string)))
    byte_list = []
    for index in range(0, len(data_string), 2):
        byte_value = int(data_string[index], 16)
        byte_value <<= 8
        byte_value |= int(data_string[index + 1], 16)
        byte_list.append(byte_value)

    return byte_list


def read_hex_file(file_name):
    ''' Read a file of hex values separated by whitespace '''
    file = open(file_name, 'r')
    read_data = []
    while True:
        data_line = file.readline()
        if not data_line:
            break

        # Split the line read into whitespace delimited bytes.
        data_string_list = data_line.split()

        # Test the first expected byte string to see if it is the expected
        # size of a whitespace delimited set of bytes.
        if len(data_string_list[0]) > 2:
            # They were not whitespace delimited after all.
            read_data.extend(data_string_to_bytes(data_string_list[0]))
        else:
            data = [int(x, 16) for x in data_string_list]
            read_data.extend(data)

    return read_data


def data_to_string(data):
    ''' Convert an integer list to a string of hex bytes separated by spaces '''
    data_str = ''
    for data_value in data:
        data_str += '%0.2x ' % data_value
    return data_str


def find_window(data, window):
    ''' Winthin a list of data, search for a match of list window '''
    pos_end = (len(data) - len(window)) + 1

    if PRINT_DEBUG > 1:
        print("find_window: data: {}  w: {}".format(data_to_string(data),
                                                    data_to_string(window)))
    for pos in range(pos_end):
        check = data[pos:pos + len(window)]
        if check == window:
            if PRINT_DEBUG > 0:
                print("Found check @   {:4} : {}".format(pos,
                                                         data_to_string(check)))
            return pos

    return NOT_FOUND


def find_unique(data, window_len):
    '''
    Find a unique list of data defined by a window of a given length.
    :data: A list of data in which to search.
    :window_len: The length of the window to search within the data.
    '''
    # Stop searching when there are less than window_len data values remaining.
    pos_end = (len(data) - window_len) + 1
    for pos in range(0, pos_end):
        window = data[pos:pos + window_len]
        if PRINT_DEBUG > 0:
            print("Checking window[{:4}]: {} ".format(pos,
                                                      data_to_string(window)))
        found_pos = find_window(data[pos + 1:], window)
        # If the window is not matched in the data following the window
        # then the window list is unique.
        if found_pos == NOT_FOUND:
            return pos

        if PRINT_DEBUG > 0:
            # Note that the found position returned from find_window() was
            # with respect to pos + 1; the first parameter value passed in.
            # Add pos + 1 to get the file offset location for debug.
            found_pos += pos + 1
            match = data[found_pos:found_pos + window_len]
            print("Found match @   {:4} : {}".format(found_pos,
                                                     data_to_string(match)))
    return NOT_FOUND


def main():
    ''' Ye ole main() '''
    global PRINT_DEBUG

    parser = argparse.ArgumentParser(
        description='Search a file for a repeating sequence'
        )

    parser.add_argument('-f', '--file', type=str, default='test_data.txt',
                        help="the file to read"
                        )
    parser.add_argument('-l', '--wlen', type=int, default=20,
                        help="the window length"
                        )
    parser.add_argument('-d', '--debug', type=int, default=0,
                        help="debugging print level"
                        )

    args = parser.parse_args()
    file_name = args.file
    window_len = args.wlen
    PRINT_DEBUG = args.debug
    print("file: '{}', window_len: {}, debug={}".format(file_name,
                                                        window_len,
                                                        PRINT_DEBUG))
    data = read_hex_file(file_name)
    if PRINT_DEBUG > 0:
        print("Read {} bytes from file '{}'".format(len(data), file_name))
    unique_pos = find_unique(data, window_len)

    if unique_pos == NOT_FOUND:
        print("No unique windows of length {} found".format(window_len))
    else:
        print("Unique window of length {} found at pos = {}".format(window_len,
                                                                    unique_pos))


if __name__ == '__main__':
    main()
