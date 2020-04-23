#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"

#define _USE_MATH_DEFINES
#include <math.h>
class Light {
public:
Light(const Vec3f& p, const float i) : position(p), intensity(i) {}
Vec3f position;
float intensity;
};

class Material {
public:
Material(const Vec3f& color) : diffuse_color(color) {}
Material() : diffuse_color() {}
private:
Vec3f diffuse_color;
friend class Renderer;
};

class Sphere {
private:
Vec3f center;
float radius;
Material material;
friend class Renderer;

bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
		Vec3f L = center - orig;
		float tca = L * dir;
		float d2 = L * L - tca * tca;
		if (d2 > radius * radius) return false;
		float thc = sqrtf(radius * radius - d2);
		t0 = tca - thc;
		float t1 = tca + thc;
		if (t0 < 0) t0 = t1;
		if (t0 < 0) return false;
		return true;
	}
public:
Sphere(const Vec3f& c, const float r, const Material& m) : center(c), radius(r), material(m) {}
};

class Renderer {
public:
void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
const int   width = 1250;
const int   height = 1000;
const float fov = 1;
std::vector<Vec3f> framebuffer(width * height);
for (size_t j = 0; j < height; j++) { // actual rendering loop
for (size_t i = 0; i < width; i++) {
float dir_x = (i + 0.5) - width / 2.;
float dir_y = -(j + 0.5) + height / 2.;    // this flips the image at the same time
float dir_z = -height / (2. * tan(fov / 2.));
framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), Vec3f(dir_x, dir_y, dir_z).normalize(), spheres, lights);
			}
		}
std::ofstream ofs; // save the framebuffer to file
ofs.open("./kartinka.ppm", std::ios::binary);
ofs << "P6\n" << width << " " << height << "\n255\n";
for (size_t i = 0; i < height * width; ++i) {

for (size_t j = 0; j < 3; j++) {
ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
			}
		}
ofs.close();
	}
private:
	bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, Vec3f& hit, Vec3f& N, Material& material) {
		float spheres_dist = std::numeric_limits<float>::max();
		for (size_t i = 0; i < spheres.size(); i++) {
			float dist_i;
			if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
				spheres_dist = dist_i;
				hit = orig + dir * dist_i;
				N = (hit - spheres[i].center).normalize();
				material = spheres[i].material;
			}
		}
		return spheres_dist < 1000;
	}
	Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
		Vec3f point, N;
		Material material;

		if (!scene_intersect(orig, dir, spheres, point, N, material)) {
			return Vec3f(0.93, 0.71, 0.85); // цвет фона
		}
		float diffuse_light_intensity = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			Vec3f light_dir = (lights[i].position - point).normalize();

			diffuse_light_intensity += lights[i].intensity * std::max(0.f, light_dir * N);
		}
		return material.diffuse_color * diffuse_light_intensity;
	}
};
int main() {
	Material  purple(Vec3f(0.60, 0.14, 0.93));//фиолет-й шарик
	Material  orange(Vec3f(0.98, 0.59, 0.19));//оранжевый 

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, -3, -18), 3, purple));
	spheres.push_back(Sphere(Vec3f(5, 2, -18), 4, orange));
	

	std::vector<Light>  lights;

	lights.push_back(Light(Vec3f(-30, 15, 20), 1.5));
	Renderer r;
	r.render(spheres, lights);

	return 0;
}
