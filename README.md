# OpenGL Minecraft Clone

## Project Description
This is a small, simple clone of the computer game Minecraft. The player can move freely through the world and place and destroy different types of blocks. They can also change the time of day, switch the flashlight on/off and zoom in/out with the camera. Some blocks have gravity, others do not, and some blocks act as point light sources.

## Project Demo
See the demo in [this YouTube video](https://youtu.be/MroHjSN1UJs).

## Control
<table>
    <tr>
        <td><b>W (hold)</b></td>
        <td>Move forward</td>
    </tr>
    <tr>
        <td><b>A (hold)</b></td>
        <td>Move right</td>
    </tr>
    <tr>
        <td><b>S (hold)</b></td>
        <td>Move backwards</td>
    </tr>
    <tr>
        <td><b>D (hold)</b></td>
        <td>Move left</td>
    </tr>
    <tr>
        <td><b>SPACE (hold)</b></td>
        <td>Move up</td>
    </tr>
    <tr>
        <td><b>SHIFT LEFT (hold)</b></td>
        <td>Move down</td>
    </tr>
    <tr>
        <td><b>CTRL LEFT (hold)</b></td>
        <td>Move faster</td>
    </tr>
    <tr>
        <td><b>MOUSE</b></td>
        <td>Look around</td>
    </tr>
    <tr>
        <td><b>SCROLL</b></td>
        <td>Switch block</td>
    </tr>
    <tr>
        <td><b>MOUSE RIGHT</b></td>
        <td>Place block</td>
    </tr>
    <tr>
        <td><b>MOUSE LEFT</b></td>
        <td>Destroy block</td>
    </tr>
    <tr>
        <td><b>C (hold)</b></td>
        <td>Zoom in</td>
    </tr>
    <tr>
        <td><b>T</b></td>
        <td>Switch time of day</td>
    </tr>
    <tr>
        <td><b>L</b></td>
        <td>Switch flashlight on/off</td>
    </tr>
</table>

## Libraries Used
- **GLFW** for creating the window, managing the OpenGL context and handling user input
- **GLAD** for loading OpenGL functions at runtime
- **GLM** for mathematical operations on vectors and matrices
- **stb_image** for loading the image files