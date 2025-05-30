/**
 * copyright (c) 2024-2025 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/SharedPtr.hpp>
#include <Core/Math/Vector3.hpp>
#include <GPU/Resource.hpp>

namespace Mach::GPU {
	struct Sampler {
		enum class Filter : u8 {
			Linear,
			Nearest,
		};
		enum class Wrap : u8 {
			Clamp,
			Repeat,
		};

		Filter min_filter = Filter::Linear;
		Filter mag_filter = Filter::Linear;

		Wrap address_u = Wrap::Clamp;
		Wrap address_v = Wrap::Clamp;
		Wrap address_w = Wrap::Clamp;
	};

	enum class Format {
		Undefined,

		R_U8,

		RGBA_U8,
		RGBA_U8_SRGB,

		RGBA_F16,
		RGBA_F32,

		BGRA_U8_SRGB,

		Depth16,
		Depth24_Stencil8,
	};

	class Texture : public Resource, public SharedPtrFromThis<Texture> {
	public:
		enum class Usage : u8 {
			TransferSrc = (1 << 0),
			TransferDest = (1 << 1),
			Sampled = (1 << 2),
			ColorAttachment = (1 << 3),
			DepthAttachment = (1 << 4),
			Swapchain = (1 << 5),
		};

		struct CreateInfo {
			Usage usage;
			Format format;
			Vector3<u32> size;
			u32 mip_levels = 1;
		};

		MACH_ALWAYS_INLINE Usage usage() const { return m_usage; }
		MACH_ALWAYS_INLINE Format format() const { return m_format; }
		MACH_ALWAYS_INLINE Vector3<u32> size() const { return m_size; }

	protected:
		explicit Texture(CreateInfo const& create_info)
			: m_usage(create_info.usage)
			, m_format(create_info.format)
			, m_size(create_info.size) {}

		Usage m_usage;
		Format m_format;
		Vector3<u32> m_size;
	};
	MACH_ENUM_CLASS_BITFIELD(Texture::Usage);
} // namespace Mach::GPU
