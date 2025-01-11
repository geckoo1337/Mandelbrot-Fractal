# Mandelbrot-Fractal

Simple Mandelbrot Fractal using OpenGL. At first, I created this little project in order to understand how to add some dependencies into a MSVC project using VCPKG. So, you need to integrate them according to these command lines (don't forget to allow the manifest use in the project properties panel) :

**vcpkg new --application**

**vcpkg add port glad**

**vcpkg add port glfw3**

**vcpkg add port glm**


This project allows the user to interact with the shader modifying its values - position, zoom and iteration. The shader is available at the address written in the main code source. You can explore the fractal using arrow keys, zooming using (left) CTRL and ALT and increase/decrease iterations with PAGE_UP & PAGE_DOWN keys. Cool stuff for a rainy day.
Mathematical beauty ++

![Mandelbrot](https://github.com/user-attachments/assets/69dae17a-f3a4-42b8-ac89-32bcdd5d556b)
