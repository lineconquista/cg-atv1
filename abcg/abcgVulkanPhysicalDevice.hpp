/**
 * @file abcgVulkanPhysicalDevice.hpp
 * @brief Header file of abcg::VulkanPhysicalDevice
 *
 * Declaration of abcg::VulkanPhysicalDevice.
 *
 * This file is part of ABCg (https://github.com/hbatagelo/abcg).
 *
 * @copyright (c) 2021--2022 Harlen Batagelo. All rights reserved.
 * This project is released under the MIT License.
 */

#ifndef ABCG_VULKAN_PHYSICAL_DEVICE_HPP_
#define ABCG_VULKAN_PHYSICAL_DEVICE_HPP_

#include <optional>

#include "abcgVulkanExternal.hpp"
#include "abcgVulkanInstance.hpp"

namespace abcg {
struct VulkanQueuesFamilies;
class VulkanPhysicalDevice;
class VulkanDevice;
class VulkanPipeline;
class VulkanSwapchain;
class VulkanWindow;
} // namespace abcg

/**
 * @brief Supported queues families.
 */
struct abcg::VulkanQueuesFamilies {
  std::optional<uint32_t> compute;
  std::optional<uint32_t> graphics;
  std::optional<uint32_t> present;
  std::optional<uint32_t> transfer;
};

/**
 * @brief A class for representing a Vulkan physical device.
 */
class abcg::VulkanPhysicalDevice {
public:
  void create(VulkanInstance const &instance, vk::SurfaceKHR const &surface,
              std::vector<char const *> const &extensions = {},
              vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1,
              bool useSeparateTransferQueue = false);
  void destroy();

  /**
   * @brief Conversion to vk::PhysicalDevice.
   */
  explicit operator vk::PhysicalDevice const &() const noexcept {
    return m_physicalDevice;
  }

  [[nodiscard]] std::optional<uint32_t>
  findMemoryType(uint32_t memoryTypeBitsRequirement,
                 vk::MemoryPropertyFlags requiredProperties) const;

  /**
   * @brief Access to abcg::VulkanInstance.
   *
   * @return Vulkan instance associated with this physical device.
   */
  [[nodiscard]] VulkanInstance const &getInstance() const noexcept {
    return m_instance;
  }

  /**
   * @brief Access to queues families.
   *
   * @return Queues families available to this physical device.
   */
  [[nodiscard]] VulkanQueuesFamilies const &getQueuesFamilies() const noexcept {
    return m_queuesFamilies;
  }

  /**
   * @brief Access to the surface.
   *
   * @return Surface associated with this physical device.
   */
  [[nodiscard]] vk::SurfaceKHR const &getSurfaceKHR() const noexcept {
    return m_surfaceKHR;
  }

  /**
   * @brief Access to number of samples.
   *
   * @return Sample count associated with this physical device.
   */
  [[nodiscard]] vk::SampleCountFlagBits getSampleCount() const noexcept {
    return m_sampleCount;
  }

  [[nodiscard]] std::optional<vk::Format>
  getFirstSupportedFormat(std::vector<vk::Format> const &candidates,
                          vk::ImageTiling tiling,
                          vk::FormatFeatureFlags features) const;

private:
  void checkQueueFamily(vk::QueueFamilyProperties const &properties,
                        uint32_t queueFamilyIndex,
                        bool useSeparateTransferQueue);
  void findQueueFamilies(bool useSeparateTransferQueue);
  [[nodiscard]] vk::SampleCountFlagBits getMaxUsableSampleCount();
  [[nodiscard]] bool
  isDeviceSuitable(std::vector<char const *> const &extensions,
                   bool useSeparateTransferQueue);
  [[nodiscard]] std::vector<char const *>
  checkExtensionsSupport(std::vector<char const *> const &extensions);

  vk::PhysicalDevice m_physicalDevice{};
  VulkanInstance m_instance{};
  vk::SurfaceKHR m_surfaceKHR{};
  vk::SampleCountFlagBits m_sampleCount{};
  VulkanQueuesFamilies m_queuesFamilies;
};

#endif