import serial

class GaltonBoardRead(object):
    """
    Wrapper class to handle interactions with the ACM counter 
    hardware on the galton board
    """
    def __init__(self, ACMx = 0):
        self.ser = serial.Serial("/dev/ttyACM{}".format(ACMx), 
                                 baudrate = 57600,
                                 bytesize = serial.EIGHTBITS, 
                                 parity   = serial.PARITY_NONE)

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
