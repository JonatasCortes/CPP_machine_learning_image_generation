# DELTA E 2000
**`ΔE2000`** is the mathmatical formula (transposed to algorythm) used in this project to establish the distance between two colors in a metric that mimics the human perception.

It performs way better than it's precessors **`ΔE76`** and **`ΔE94`**, and is currently the best option for the task, used in various well estabilished color treatment libraries.

## How it works

The process of metrifying the distance between two colors as we see is not a trivial task, but follows a guide of steps that won't fail if correctly aplied.

> [!WARNING]
> In this explanation I won't bother to explain the meaning of every "magic constant" that apears trought the implementation (since most of them not even I comprehend), but all of them can be found in the various articles mentioned in this documentation.

First of all, we can't jump to the formula right away, since the usual **RGB** pixels extracted directly from images are not the ones we are going to use.

We have to apply the following conversions before even thinking of the **`ΔE2000`** formula:
- RGB ---> Linearized RGB
- Linearized RGB ---> CIE_XYZ
- CIE_XYZ ---> CIE_LAB

And only then we can use the "CIE_LAB pixels" in our calculations.

### Linearized RGB

First of all, what is the problem with conventional RGB in terms of calculation?

It may seem counterintuitive, but RGB values have a **non linear** light perception. For instance, take the two following colors:

<div style="display: flex; gap: 10px;">
<div>
<div style="width:100px; height:100px; background: rgb(255, 0, 0);"></div>
<p style="color: rgb(255, 0, 0);">color 1</p>
</div>
<div>
<div style="width:100px; height:100px; background: rgb(128, 0, 0);"></div>
<p style="color: rgb(128, 0, 0);">color 2</p>
</div>
</div>

The first one is pure red (255, 0, 0) and the seccond is "half pure red" (128, 0, 0), but as you can see, it apears to have way less than half the light energy, and that's because it has indeed.

The actual half red would be closer to (187, 0, 0):

<div style="width:100px; height:100px; background: rgb(187, 0, 0);"></div>
<p style="color: rgb(187, 0, 0);">color 3</p>

And thats exactly were the problem lies, because CIE_XYZ and CIE_LAB both rely on linear perception of light.

With that in mind, we can first normalize the RGB values (divide by 255) so that their range stays between 0 and 1. Then, we can apply the linearizing function, defined as:

![linear-rgb-conversion-formula](images/linear_rgb_conversion_formula.jpg)
Reference: [How-to-convert-between-srgb-and-ciexyz](https://www.image-engineering.de/library/technotes/958-how-to-convert-between-srgb-and-ciexyz).



### CIE_XYZ
With linear RGB in hands, now we raise the question: How can we make the colors used in our calculations agnostic to device?

The same image, if displayed in various devices, will have, inevitably, color missmatches due to hardware and software particularities. There are also many different RGB spectrums, each one with specific primary reference colors, like sRGB, Adobe RGB and ProPhoto RGB.

To adress this issue, the **_Commission Internationale de l’Eclairage_ (CIE)** established the CIE_XYZ color space, with the premisse of it being universaly independent from device specifications, focusing exclusively on phisical perception. 

In resume, the CIE_XYZ is like if every application in the world could see color in the same perspective.

![CIE-XYZ-color-soace](images/cie_xyz_color_space.jpg)

The conversion process from linear RGB to CIE_XYZ is as simple as a matrix product:

![](images/cie_xyz_conversion_matrix.jpg)
Reference: [How-to-convert-between-srgb-and-ciexyz](https://www.image-engineering.de/library/technotes/958-how-to-convert-between-srgb-and-ciexyz).
> [!WARNING]
> The values used in the matrix are only valid under the reference of D65 as the "Pure White", wich is what we are using for this project.

### CIE_LAB