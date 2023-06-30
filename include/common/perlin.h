#ifndef PERLIN_H
#define PERLIN_H

#include "common.h"

class perlin{
    public:
        perlin() {
            ranvec = new vecf3[point_count];
            for (int i = 0; i < point_count; ++i) {
                ranvec[i] = unit_vector(vecf3::random(-1, 1));
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }
        ~perlin(){
            delete[] ranvec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        double noise(const pointf3& p)const{
            //线性插值的perlin noise

            //取小数
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            //Hermite cubic smoothing
            // u = u * u * (3 - 2 * u);
            // v = v * v * (3 - 2 * v);
            // w = w * w * (3 - 2 * w);

            //取整数
            auto i = static_cast<int>(floor(p.x()));
            auto j = static_cast<int>(floor(p.y()));
            auto k = static_cast<int>(floor(p.z()));
            vecf3 c[2][2][2];

            for (int di = 0; di < 2; di++)
                for (int dj = 0; dj < 2; dj++)
                    for (int dk = 0; dk < 2; dk++)
                        c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^
                                                 perm_y[(j + dj) & 255] ^
                                                 perm_z[(k + dk) & 255]];

            return trilinear_interp(c, u, v, w);//存在负值
        }

        double checker_noise(const pointf3 p)const{//会有格子的效果
            //hash 取值,这种方式保证了局部的连续性
            auto i = static_cast<int>(4 * p.x()) & 255;
            auto j = static_cast<int>(4 * p.y()) & 255;
            auto k = static_cast<int>(4 * p.z()) & 255;

            return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];//hash 取值
        }

        double turb(const pointf3& p, int depth = 7) const {
            //湍流
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(temp_p);//不同频率噪声的叠加
                weight *= 0.5;
                temp_p *= 2;
            }

            return fabs(accum);//保证了返回值为正
        }

    private:
        static const int point_count = 256;//所有类对象共享一个静态变量
        vecf3* ranvec;
        double* ranfloat;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm() {
            // 成员函数声明为static意味着这个函数与类的实例无关，而是与类本身关联。
            auto p = new int[point_count];

            for (int i = 0; i < perlin::point_count; i++)//直接使用静态变量而不依赖于类
                p[i] = i;

            permute(p, point_count);

            return p;
        }

        static void permute(int* p, int n) {
            for (int i = n - 1; i > 0; i--) {
                int target = random_int(0, i);//随机数范围越来越小
                int tmp = p[i];//随机交换数组元素
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static double trilinear_interp(vecf3 c[2][2][2], double u, double v, double w) {
            // auto accum = 0.0;
            // for (int i = 0; i < 2; i++)
            //     for (int j = 0; j < 2; j++)
            //         for (int k = 0; k < 2; k++)
            //             accum += (i * u + (1 - i) * (1 - u)) *
            //                      (j * v + (1 - j) * (1 - v)) *
            //                      (k * w + (1 - k) * (1 - w)) * c[i][j][k];
            auto uu = u * u * (3 - 2 * u);
            auto vv = v * v * (3 - 2 * v);
            auto ww = w * w * (3 - 2 * w);
            auto accum = 0.0;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++) {
                        //线性插值,与周围 8 个点的距离成正比
                        vecf3 weight_v(u - i, v - j, w - k);
                        accum +=    (i * uu + (1 - i) * (1 - uu)) * 
                                    (j * vv + (1 - j) * (1 - vv)) * 
                                    (k * ww + (1 - k) * (1 - ww)) * 
                                    dot(c[i][j][k], weight_v);
                    }
            return accum;
        }
};

#endif