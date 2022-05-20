#pragma once

namespace abc
{
	class Model
	{
	public:
		Model(const std::vector<std::string>& meshPaths);
		Model(const std::vector<std::string>& meshPaths, const std::vector<std::string>& texturePaths);

		~Model();
	};
}

