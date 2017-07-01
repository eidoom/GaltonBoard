from sys import stderr

class FakeSerial(object):
    """
    Fake data source to use if serial connection to
    the hardware counter is not available.
    """
    def __init__(self, *args, **kwargs):
        self.data = [1]*13
    def write(self, msg, *args, **kwargs):
        """Increment fake data each time we're called"""
        if msg == 'C':
            self.data = [0]*13
        else:
            self.data = [self.data[0]+1]*13
    def readline(self, *args, **kwargs):
        """Return current fake data"""
        data_str = ','.join(map(str,self.data))
        return data_str + ',    %s'%sum(self.data)

try:
    from serial import Serial, EIGHTBITS, PARITY_NONE
except:
    # use a fake serial data source
    stderr.write('Serial module not found. Using fake data!\n')
    EIGHTBITS, PARITY_NONE = None, None
    Serial = FakeSerial

class GaltonBoardRead(object):
    """
    Wrapper class to handle interactions with the ACM counter 
    hardware on the galton board
    """
    def __init__(self, ACMx = 0):
        tty="/dev/ttyACM{}".format(ACMx)
        try:
            self.ser = Serial(tty, 
                              baudrate = 57600,
                              bytesize = EIGHTBITS, 
                              parity   = PARITY_NONE)
        except:
            stderr.write('Serial connection to %s failed. Using fake data!\n'%tty)
            self.ser = FakeSerial()

    def reset_counters(self):
        self.ser.write("C")

    def read_from_counters(self):
        self.ser.write("S")
        return self.ser.readline().strip()

    def parse_count(self, counter_output):
        """
        Parse the output of the counter hardware.

        The output comes in the form
        #,#,#,#,#,#,#,#,#,#,#,#,#,    #
        where the first 13 numbers are individual bin counts 
        and the last number is a total
        """
        counts, total = counter_output.split()
        # Remove the trailing comma, then split into individual bins
        counts = counts[:-1].split(",")

        total = int(total)
        counts = map(int, counts)
        return counts, total

    def print_bins(self):
        counter_output = self.read_from_counters()
        counts, _ = self.parse_count(counter_output)
        print ",".join(map(str, counts))

    def print_total(self):
        counter_output = self.read_from_counters()
        _, total = self.parse_count(counter_output)
        print total

    def formatted_count(self):
        """
        Return a formatted count, parseable by the galton simulators.

        The format is comma-separated integers, preceded by 'a' and followed by 'z',
        to be able to detect partial transfers
        """
        # strip off the total
        count = self.read_from_counters().split()[0]
        count = 'a,%sz' % count
        return count

    def numeric_count(self):
        """Return a list of integer bucket counts"""
        # strip the total figure
        count = self.read_from_counters().split()[0]
        # remove blank after trailing comma
        count = count.split(',')[:-1]
        count = map(int,count)
        return count

if __name__ == "__main__":
    gb = GaltonBoardRead()
    while True:
        print 'Waiting for <enter>',
        raw_input()
        print  "Total:"
        gb.print_total()
        print "Bins:"
        gb.print_bins()
        gb.reset_counters()
