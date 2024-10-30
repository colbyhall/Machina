/**
 * copyright (c) 2024 colby hall <me@cobeh.com>
 *
 * this software is released under the mit license.
 */

#pragma once

#include <Core/Containers/Shared.hpp>
#include <Core/Math/Vector3.hpp>
#include <GPU/Resource.hpp>

namespace Grizzly::GPU {
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

	class Texture : public Resource, public SharedFromThis<Texture> {
	public:
		enum class Usage : u8 {
			TransferSrc = (1 << 0),
			TransferDest = (1 << 1),
			Sampled = (1 << 2),
			ColorAttachment = (1 << 3),
			DepthAttachment = (1 << 4),
			Swapchain = (1 << 5),
		};

		enum class Format {
			Undefined,

			R_U8,

			RGB_U8,
			RGB_U8_SRGB,
			RGBA_U8,
			RGBA_U8_SRGB,

			RGBA_F16,

			RGBA_F32,

			BGR_U8_SRGB,

			Depth16,
			Depth24_Stencil8,
		};

		struct CreateInfo {
			Usage usage;
			Format format;
			Vector3<u32> size;
		};

		GRIZZLY_ALWAYS_INLINE Usage usage() const { return m_usage; }
		GRIZZLY_ALWAYS_INLINE Format format() const { return m_format; }
		GRIZZLY_ALWAYS_INLINE Vector3<u32> size() const { return m_size; }

	protected:
		Usage m_usage;
		Format m_format;
		Vector3<u32> m_size;
	};
	GRIZZLY_ENUM_CLASS_BITFIELD(Texture::Usage);
} // namespace Grizzly::GPU
