import urllib2

class BrokenData(Exception):
    pass

def read():
    url = 'http://10.10.10.10/galton_counts.txt'

    data = urllib2.urlopen(url)
    counts = data.readline()
    data.close()

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
    data = read()
    print data
