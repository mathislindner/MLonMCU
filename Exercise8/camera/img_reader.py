import serial
import numpy as np
import matplotlib.pyplot as plt

def parse_image_rgb565(img, x, y):
    pixels = np.zeros((x*y, 3))
    for i in range(0, x*y*2, 2):
        # The pixel composition is the followinf: bbbbbggg gggrrrrr
        # therefore we have to do some masking
        b = img[i+1] & 0x1f
        g = ((img[i] & 0x07) << 3) | ((img[i+1] & 0xe0) >> 5)
        r = (img[i] & 0xf8) >> 3
        pixels[i//2, 0] = int(r / 31 * 255) # Red and Blue are encoded in 5 bits
        pixels[i//2, 1] = int(g / 63 * 255) # Green has a bit more!
        pixels[i//2, 2] = int(b / 31 * 255) 
    # Get the array dimensions right
    pixels = np.reshape(pixels, (x_size, y_size, 3), order='C')
    print(img[0])
    print([img[1]])
    print(pixels[0,0,0])
    print(pixels[0,0,1])
    print(pixels[0,0,2])
    return pixels


ser = serial.Serial('/dev/cu.usbmodem14102', 115200, timeout=30)  # open serial port
print(f"Succesfully opened: {ser.name}")

# Flushing the buffer
ser.read_all()

buffer = bytearray("", "utf8")
tocken = bytearray("New image", "utf8")
# Continously reading until start message received
while tocken not in buffer:
        buffer = ser.readline()

# Now we are synchronised!
# We know the content of the payload, so we read and parse it

# There are 5 bytes encoding the image dimension (2 byte each dimension)
# And one byte encoding the lenght of the pixel formats
img_resol = ser.read(5) # timeout
# We are now dealing with bytes
# Which are not encoded to make sense to humans
# In this case we know that in the first two bytes there is an integer,
# and they are big ending. So we proceed to decode it!
x_size = int.from_bytes(img_resol[0:2], byteorder='big')
y_size = int.from_bytes(img_resol[2:4], byteorder='big')
pixel_format_len = int.from_bytes(img_resol[4:5], byteorder='big')
print(f"X dimension: {x_size}, Y dimension: {y_size}")

# And now we receive the pixel format
# Which is encoded in a string
img_format = ser.read(pixel_format_len) # timeout
print("Image format: ", img_format.decode("utf-8"))

# Now we expect 4 bytes encoding the lenght of the image
img_dim = ser.read(4)
img_dim = int.from_bytes(img_dim, byteorder='big')
# We are using the RGB565, so a total of 16bits, or 2 bytes per image
# At a resolution of 64*64 pixels is a total of 8192 bytes
print("Total image dimension: ", img_dim)

# Now we can finally acquire the image...
img = ser.read(img_dim)
# But we still have to parse it!
# Every 2 bytes there is a pixel
img = parse_image_rgb565(img, x_size, y_size)
plt.imshow(img/255)
plt.show()