import urllib2

class BrokenData(Exception):
    pass

def read_board_data():
    url = 'http://10.10.10.10/galton_counts.txt'

    try:
        conn = urllib2.urlopen(url, timeout=3)
        counts = conn.readline()
        conn.close()
    except:
        return [0]*13

    counts = counts.rstrip().split(',')
    if counts[0] != 'a' or counts[-1] != 'z':
        raise BrokenData()
    counts = counts[1:-1]

    try:
        counts = map(int,counts)
    except ValueError:
        raise BrokenData()

    return counts

if __name__ == '__main__':
    data = read_board_data()
    print data
