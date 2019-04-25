#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstdlib>
#include <cstring>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

#define NDEBUG 0

const std::vector<const char*> validationLayers =
  {
   "VK_LAYER_LUNARG_standard_validation"
  };

#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

class HelloTriangleApp {
public:
  void run();
private:
  GLFWwindow *window;
  VkInstance instance;

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    
    return VK_FALSE;
  }

  std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    
    if (enableValidationLayers) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    return extensions;
  }

  bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (auto layerName : validationLayers) {
      bool layerFound = false;
      
      for (const auto& layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
	  layerFound = true;
	  break;
        }
      }
      
      if (!layerFound) {
        return false;
      }
    }
    
    return true;
  }
  
  void createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
      throw std::runtime_error("Validation layers requested, but not available");
    }
    
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello, Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (enableValidationLayers) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("Failed to create VK instance");
    }
  }
  
  void initVulkan() {
    createInstance();
  }
  
  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }
  
  void cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  }
  
  void initWindow() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }
};

void HelloTriangleApp::run() {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

int main() {

  HelloTriangleApp app;

  try {
    app.run();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
