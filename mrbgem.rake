MRuby::Gem::Specification.new('mruby-autoclick') do |spec|
  spec.license = 'MIT'
  spec.authors = 'expeditiousRubyist'

  spec.cc.include_paths << "#{build.root}/src"
  spec.linker.libraries << "user32"
end