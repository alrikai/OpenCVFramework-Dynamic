Pod::Spec.new do |spec|
  spec.name         = "OpenCV"
  spec.version      = "4.3.0"
  spec.summary      = "OpenCV dynamic framework"
  spec.description  = "From https://github.com/opencv/opencv"
  spec.homepage     = "https://github.com/alrikai/OpenCVFramework-Dynamic.git"
  spec.license      = { :type => '3-clause BSD', :file => 'opencv-4.3/LICENSE' } 
  spec.author       = "https://opencv.org/" 
  spec.platform     = :ios
  spec.ios.deployment_target = "11.1"
  spec.source       = { 
      :git => "https://github.com/alrikai/OpenCVFramework-Dynamic.git", 
      :tag => "v#{spec.version.to_s}" 
  }

  spec.prepare_command = <<-CMD
      git reset --hard
      git fetch opencv 4.3.0
      git subtree pull --prefix opencv-4.3 opencv 4.3.0 --squash
      python2.7 opencv-4.3/platforms/ios/build_framework.py opencv-ios-dynamic --dynamic
      cp -a opencv-ios-dynamic/opencv2.framework ./opencv2.framework
  CMD
  
  spec.source_files = "opencv2.framework/Headers/**/*{.h,.hpp}"
  spec.preserve_paths = "opencv2.framework"
  spec.vendored_frameworks = "opencv2.framework"
  spec.requires_arc = false
  #spec.public_header_files = "mobile/ios/HVImageCheck/HVImageCheck/**/*.h"
  #spec.preserve_paths = "mobile/ios/HVImageCheck/HVImageCheck/Imagecheck/Imagecheck.modulemap", "mobile/ios/HVImageCheck/Dependencies/*"
	spec.ios.frameworks = [
    "AssetsLibrary",
    "AVFoundation",
    "CoreGraphics",
    "CoreMedia",
    "CoreVideo",
    "Foundation",
    "QuartzCore",
    "UIKit"
	]

  spec.libraries = "c++"
  spec.pod_target_xcconfig = {
      "CLANG_CXX_LANGUAGE_STANDARD" => "c++17",
      "CLANG_CXX_LIBRARY" => "libc++",
  }
end
