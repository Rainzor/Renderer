#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class texture {
public:
    virtual color value(double u, double v, const pointf3 &p) const = 0;

    virtual color value(const ray &r) const {
        return color(0, 0, 0);
    }
};

class solid_color : public texture {
public:
    solid_color() {}

    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    virtual color value(double u, double v, const pointf3 &p) const override {
        return color_value;
    }

    virtual color value(const ray &r) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd) : even(_even), odd(_odd) {}

    checker_texture(color c1, color c2) : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const pointf3 &p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

public:
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class noise_texture : public texture {
public:
    noise_texture() {}

    noise_texture(double sc) : scale(sc) {}

    virtual color value(double u, double v, const pointf3 &p) const override {
        //return color(1, 1, 1) * noise.turb(scale * p);  // 返回值与p有关而与纹理坐标无关
        //接近大理石的纹路，变化随着相位而改变
        return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

public:
    perlin noise;
    double scale;//坐标的缩放
};

class image_texture : public texture {//从已给图片中获取纹理
public:
    const static int bytes_per_pixel = 3;

    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char *filename) {
        auto components_per_pixel = bytes_per_pixel;
        //stb读取文件，以char形式存储
        //输入的是文件名，宽、高、通道数
        //最后一个参数是指定通道数，因为我们只用了RGB三个通道，所以是3；如果0，就是用图片的通道数
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    //输入纹理坐标，返回颜色
    virtual color value(double u, double v, const pointf3 &p) const override {
        //如果没有加载成功，就用紫色代替
        if (data == nullptr) {
            return color(0, 1, 1);
        }

        //clamp input texture coordinates to [0,1] x [0,1]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);//flip v to image coordinates:因为图像的坐标系是从上到下的，而纹理坐标是从下到上的

        auto i = static_cast<int>(u * width);//纹理坐标转化为图像坐标
        auto j = static_cast<int>(v * height);

        //clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        //第j行，第i列，第0个像素
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
        //将像素值转化为[0,1]之间的值
        const auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};

#endif