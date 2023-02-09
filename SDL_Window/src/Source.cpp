#include "sdltemplate.h"
#include <iostream>
#include "color.h"
#include "utility.h"
#include "hittable_list.h"
#include "Sphere.h"
#include "camera.h"
#include "Materials.h"


vec3 ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return vec3(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return vec3(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);

}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

	return world;
}

int main() {

	//IMAGE
	const int samples_per_pixel = 100;
	const int max_depth = 50;
	const auto aspect_ratio = 3.0 / 2.0;

	const int width = 1200 ;
	const int height = static_cast<int>(width / aspect_ratio);
	
	std::cout << "P3\n" << width << " " << height << "\n255\n";
	
	sdltemplate::sdl("Ray Tracer", width, height);
	sdltemplate::loop();


	//CAMERA
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	//WORLD

	
	hittable_list world = random_scene();



	for (int y = height - 1; y >= 00; y--) {
		for (int x = 0; x < width; x++) {

			vec3 pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (x + random_double()) / (width - 1);
				auto v = (y + random_double()) / (height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			vec3 rgb = write_color(pixel_color, samples_per_pixel);
			//std::cout << ir << ib << " " << ig << " " << ib << "\n";
			sdltemplate::setDrawColor(sdltemplate::createColor(rgb.r(), rgb.g(), rgb.b(), 255));
			sdltemplate::drawPoint(x,height -  y);
		}
	}

	while(sdltemplate::running){
		sdltemplate::loop();
	}
	
}
