#include "renderer_stdafx.h"

#include <LowRenderer/Color.h>

using namespace Renderer;
using namespace LowRenderer;

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
	m_r(r),
	m_g(g),
	m_b(b),
	m_a(a)
{}

Color::Color(const float r, const float g, const float b, const float a) :
	m_r(static_cast<unsigned char>(r * 255)),
	m_g(static_cast<unsigned char>(g * 255)),
	m_b(static_cast<unsigned char>(b * 255)),
	m_a(static_cast<unsigned char>(a * 255))
{}

Color::Color(const Core::Maths::vec3f& rgb, const float a) :
	m_r(static_cast<unsigned char>(rgb.x * 255)),
	m_g(static_cast<unsigned char>(rgb.y * 255)),
	m_b(static_cast<unsigned char>(rgb.z * 255)),
	m_a(static_cast<unsigned char>(a * 255))
{}

Color::Color(const Core::Maths::vec4f& rgba) :
	m_r(static_cast<unsigned char>(rgba.x * 255)),
	m_g(static_cast<unsigned char>(rgba.y * 255)),
	m_b(static_cast<unsigned char>(rgba.z * 255)),
	m_a(static_cast<unsigned char>(rgba.w * 255))
{}

Color::Color(const Core::Maths::vec3<unsigned char>& rgb, const unsigned char a) :
	m_r(rgb.x),
	m_g(rgb.y),
	m_b(rgb.z),
	m_a(a)
{}

Color::Color(const Core::Maths::vec4<unsigned char>& rgba) :
	m_r(rgba.x),
	m_g(rgba.y),
	m_b(rgba.z),
	m_a(rgba.w)
{}