# Machine Learning On Microcontrollers
## Project: Face segmentation on MAXIM MAX78000
### Introduction
This is a brief overview on the project, there is pdf of a presentation in /project folder.
As part of the course we were required to find a project and implement it on a microcontroller. Most projects on a microcontroller involve a CNN, to bring novelty but still leverage the architecture of the MAXIMM78000 I chose to implement a U-Network.
My idea was to be able to segment a face from a picture and then use the segmented face to do some other task. For example, I could use the segmented face to do face recognition or to do emotion recognition. For the training of the U-Net I used the CelebAMask-HQ Dataset.
Since the maximum input size of the network is 90×91x3, I created a dataloader to make the image to be 88x88x3.

### Results
Here are a few results of the segmentation:
![celeb_seg](/project/docs/images/celebhqsegmentation.png)

These are the results:
|Data-Set|Size  |IoU     |
|--------|------|--------|
|Training| 22746| 0.91390|
|Test    | 4836 | 0.88632|

### Work that was done
The main things that were done are:
- adding a dataloader to the project
- changing the existing U-Net to fit on the MC
- adding a new loss function (Dice Loss)
- adding the status of the training 
- adding a new metric (IoU)
- training the network
- Synthesizing the network using the MAXIM toolchain.
- Testing the network on the MAXIM board

### Side notes
The project is not finished, there are still a few things that were started but could not be verified due to time constraints:
- adding other things to segment (like eyes, mouth, etc.)
- adding a loss function for the other things to segment

## Exercises 
Details on the exercises can be found in /exercises or on [the courses website](https://iis-students.ee.ethz.ch/lectures/machine-learning-on-microcontrollers-page/)

