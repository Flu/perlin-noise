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
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class HelloTriangleApp {
public:
  void run();
private:
  GLFWwindow *window;
  VkInstance instance;

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

    uint32_t glfwExtensionCount = 0;
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:" << std::endl;
    for (const auto& extension : extensions) {
      std::cout << "\t" << extension.extensionName << std::endl;
    }
    
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensionsNotSupported;
    for (int index = 0; index < glfwExtensionCount; index++) {
      bool isSupported = false;
      for (int ext = 0; ext < extensionCount; ext++) {
	if (strcmp(glfwExtensions[index], extensions[ext].extensionName) == 0)
	  isSupported = true;
      }
      if (!isSupported) {
	extensionsNotSupported.push_back(glfwExtensions[index]);
      }
    }

    if (extensionsNotSupported.size() != 0) {
      std::cout << "Extensions not supported:" << std::endl;
      for (const auto& ext : extensionsNotSupported) {
	std::cout << "\t" << ext << std::endl;
      }
    }

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
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
