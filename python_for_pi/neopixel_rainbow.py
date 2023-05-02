import board
import neopixel
import time
DATA_PIN = board.D10
N_PIXELS = 30

pixels = neopixel.NeoPixel(DATA_PIN, N_PIXELS, auto_write=False)

r_arr = [ [148, 0, 211],
          [75, 0, 30],
          [0, 0, 255],
          [0, 255, 0],
          [255, 255, 0],
          [255, 127, 0],
          [255, 0, 0] ]

# Pattern 1
# Entire neopixel fills with every colour of the rainbow
for i in range(len(r_arr)):
    pixels.fill((r_arr[i][0], r_arr[i][1], r_arr[i][2]))
    pixels.show()
    time.sleep(1)

pixels.deinit()
pixels = neopixel.NeoPixel(DATA_PIN, N_PIXELS, auto_write=False)

# Pattern 2
# The pixel slowly fills as a multi-rainbow design
def check_r_pos(r_pos: int) -> int:
    if (r_pos < -6):
        r_pos = 0
    return r_pos

def set_pixel(pos: int, r_pos:int) -> None:
    pixels[pos] = (r_arr[r_pos][0], r_arr[r_pos][1], r_arr[r_pos][2])
    if pos-1 > -1:
        r_pos = check_r_pos(r_pos)
        set_pixel(pos-1, r_pos-1)

def main():
    for i in range(pixels.n):
        for j in range(len(r_arr)):
            pixels[i] = (r_arr[j][0], r_arr[j][1], r_arr[j][2])
            if i-1 > -1:
                set_pixel(i-1, j-1)
            pixels.show()
            time.sleep(0.5)

main()

pixels.deinit()
pixels = neopixel.NeoPixel(DATA_PIN, N_PIXELS, auto_write=False)

# set_pixel(pixels.n, 0)
# pixels.show()