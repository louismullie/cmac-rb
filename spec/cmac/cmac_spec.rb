require 'spec_helper'
require 'cmac-rb'

describe CMAC do

  it 'should fail if key is empty' do
    
    expect { CMAC::Digest.new('') }.to raise_error
    
  end

  it 'should pass RFC spec' do
    
    examples = [
      ['2b7e151628aed2a6abf7158809cf4f3c', '', 'bb1d6929e95937287fa37d129b756746'],
      ['2b7e151628aed2a6abf7158809cf4f3c', '6bc1bee22e409f96e93d7e117393172a', '070a16b46b4d4144f79bdd9dd04a287c'],
      ['2b7e151628aed2a6abf7158809cf4f3c', '6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411', 'dfa66747de9ae63030ca32611497c827'],
      ['2b7e151628aed2a6abf7158809cf4f3c', '6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710', '51f0bebf7e3b9d92fc49741779363cfe']
    ]
    
    examples.each do |example|
      
      key, plaintext, expected =
      example.map { |x| [x].pack('H*') }
      
      digest = CMAC::Digest.new(key)
      tag = digest.update(plaintext)
      
      tag.should eql expected
      
    end
    
  end

end
