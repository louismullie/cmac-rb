$:.push File.expand_path('../lib', __FILE__)
require 'cmac/version'

Gem::Specification.new do |s|
  
  s.name        = 'cmac'
  s.version     = CMAC::VERSION
  s.authors     = ['Louis Mullie']
  s.email       = ['louis.mullie@gmail.com']
  
  s.homepage    = 'https://github.com/cryodex/cmac-rb'
  s.summary     = %q{ Ruby C extension for the AES-CMAC keyed hash function. }
  s.description = %q{ Ruby C extension for the AES-CMAC keyed hash function. }

  s.files = Dir.glob('lib/**/*.rb') +
  Dir.glob('ext/**/*.{c,h,rb}')

  s.extensions << 'ext/cmac/extconf.rb'
  s.add_development_dependency 'rspec', '~> 2.12.0'
  s.add_development_dependency 'rake'
  
end
