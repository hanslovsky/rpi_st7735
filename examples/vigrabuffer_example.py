import framebuffer
import vigra
import numpy as np

##### EXAMPLE CODE
##### IF YOU WANT TO USE THIS CODE, MODIFY THE MAIN PART ACCORDINGLY
#####   - CHOOSE PROPER IMAGE PATH
#####   - CHOOSE SETTINGS APPROPRIATE TO YOUR MONITOR
##### YOU WILL NEED TO HAVE ACCESS TO /dev/fb0 (easiest way to do so: add
##### your user to the group video (sudo usermod -aG video username)
##### DO NOT RUN THIS IN AN X TERMINAL (USE CTRL + ALT + F1 FOR EXAMPLE)


class Reposition(object):
    def __init__(self,
                 steps=[1,1]):
        self.steps = steps

    def update(self, object_size, position, borders_high, borders_low):
        old_position = list(position)
        for i in xrange(len(self.steps)):
            if position[i] + object_size[i] + self.steps[i] >= borders_high[i] or \
               position[i] + self.steps[i] < borders_low[i]:
                self.steps[i] *= -1
            position[i] += self.steps[i]

        return old_position, position


class MovingDrawer(object):
    def __init__(self,
                 basic_image,
                 moving_image,
                 device_name,
                 cutout):
        self.basic_image = basic_image
        self.moving_image = moving_image
        self.device = framebuffer.VigraBuffer(device_name)
        self.cutout = cutout
        self.position = [0,0]
        self.borders_high = [self.device.getInfo().width, self.device.getInfo().height]
        self.borders_low = [0, 0]

    def draw_first(self):
        x,y = self.position
        self.device.drawRectangle(basic_image, x, y)
        self.device.drawRectangle(np.copy(moving_image[:, :self.cutout[0], :self.cutout[1]], order='F'), x, y)


    def update(self, reposition):
        old_position, self.position = reposition.update(self.cutout, self.position, self.borders_high, self.borders_low)

        self._restore_basic(old_position, self.position)
        
        self.device.drawRectangle(np.copy(self.moving_image[:,
                                                            self.position[0]:self.position[0]+self.cutout[0],
                                                            self.position[1]:self.position[1]+self.cutout[1]],
                                           order='F'),
                                  self.position[0],
                                  self.position[1],)

        
    def _restore_basic(self, old_position, new_position):
        diff = np.array(self.position) - np.array(old_position)
        direction = np.sign(diff)
        if direction[1] > 0:
            slicing = slice(old_position[1], old_position[1]+diff[1])
            start = old_position[1]
        else:
            slicing = slice(old_position[1]+self.cutout[1]+diff[1], old_position[1]+self.cutout[1])
            start = old_position[1] + self.cutout[1] + int(diff[1])
        self.device.drawRectangle(np.copy(self.basic_image[:,
                                                           old_position[0]:old_position[0]+self.cutout[0],
                                                           slicing],
                                           order='F'),
                                  old_position[0],
                                  start)

        if direction[0] > 0:
            slicing = slice(old_position[0], old_position[0]+diff[0])
            start = old_position[0]
        else:
            slicing = slice(old_position[0]+self.cutout[0]+diff[0], old_position[0]+self.cutout[0])
            start = old_position[0] + self.cutout[0] + int(diff[0])
        self.device.drawRectangle(np.copy(self.basic_image[:,
                                                           slicing,
                                                           old_position[1]:old_position[1]+self.cutout[1]],
                                           order='F'),
                                  start,
                                  old_position[1])        
        


if __name__ == "__main__":
    import time
    image = vigra.readImage('/home/phil/Downloads/1920x1080-wallpaper.jpg')
    image_gs = np.mean(image, axis=-1)
    edges_single = vigra.filters.gaussianGradientMagnitude(image_gs, sigma=1)
    image = np.append(image, np.zeros(image.shape[:-1] + (1,), dtype=image.dtype), axis=-1)

    
    
    basic_image = np.array(image.transpose((2,0,1)), dtype=np.uint8, order='F')
    image_edges = np.zeros(basic_image.shape, dtype=np.uint8, order='F')
    for i in xrange(3):
        image_edges[i,...] = edges_single
    moving_image = np.zeros(basic_image.shape, dtype=np.uint8, order='F')
    rgb_image = np.zeros((4,600,200), dtype=np.uint8, order='F')
    rgb_image[0, :200, :] = 255
    rgb_image[1, 200:400, :] = 255
    rgb_image[2, 400:600, :] = 255
    moving_image[0, :640, :] = 255
    moving_image[1, 640:1280, :] = 255
    moving_image[2, 1280:, :] = 255
    # fb = framebuffer.VigraBuffer('/dev/fb0')
    # fb.drawRectangle(basic_image, 0, 0)
    reposition = Reposition([10, 2])
    drawer = MovingDrawer(basic_image, image_edges, '/dev/fb0', [1000, 500])
    drawer.draw_first()
    # fb.drawRectangle(np.copy(moving_image[:,:1000, :200], order='F'), 0, 200)
    # fb.drawRectangle(moving_image, 0, 0)
    while True:
        time.sleep(0.01)
        drawer.update(reposition)
    # fb.drawRectangle(rgb_image, 0, 0)
    
