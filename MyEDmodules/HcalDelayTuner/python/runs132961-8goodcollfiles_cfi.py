import FWCore.ParameterSet.Config as cms
source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/F6467432-9145-DF11-B853-003048635B88.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/E6FE7EEB-8E45-DF11-830F-00E08178C027.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/E4996C2B-9145-DF11-8D09-00E081791819.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/DAD1E9F3-0D46-DF11-8E58-00E08178C13D.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/CCF83E84-9245-DF11-A1D2-00E081791819.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/B42475CF-9345-DF11-82B3-0025B3E05DFE.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/84493FC7-9145-DF11-AA28-0025B3E05CF8.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/7A19BAC9-8F45-DF11-A10E-0025B31E3C1C.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/7642A68A-9045-DF11-8ABF-0025B3E065DC.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/52E09475-9C45-DF11-8F86-003048D47A2E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/4C7D6F0F-9546-DF11-A426-00E081791851.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/46207C75-9A45-DF11-9CE6-003048673F54.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/4081F1CE-9345-DF11-B0B5-003048673F54.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/327FC3D0-8346-DF11-B66E-00E08178C0D9.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/2237B426-9145-DF11-AFAE-003048D46038.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/968/085F429F-9045-DF11-A4A8-0025B31E3C1C.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/966/0EF37D45-9245-DF11-8E4D-00E08178C027.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/E2857AB7-8F45-DF11-8DB5-0025B3E05DB6.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/D2AFB386-9045-DF11-A70B-002481E14F86.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/CC26CEF5-8546-DF11-BAFD-00E0817918A5.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/CA56DF8C-9245-DF11-B16B-003048D4605E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/A07372E3-9245-DF11-9DD7-00E081791733.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/90333F13-9145-DF11-8332-00E081791853.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/560CD210-9945-DF11-8745-003048D47A84.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/965/08EA2FB2-9145-DF11-9210-0025B3E05BFC.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/964/FE039C51-8045-DF11-A692-0025B3E05DB6.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/963/72CCDD15-7F45-DF11-A6B3-00304866C51E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/F0AC80CF-8745-DF11-8389-00E0817918D3.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/EEFED391-8645-DF11-A858-0025B3E05CBC.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/DC6F2C2D-8945-DF11-B91E-00E08178C117.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/CE9D75CF-8745-DF11-82BA-00E08178C027.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/C81922A8-7C46-DF11-9B3C-003048673E9A.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/C2C7FCCF-8745-DF11-9A92-00E08178C037.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/B056F221-8745-DF11-9DA2-00E08178C097.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/AEF7C629-8945-DF11-A32E-0025B3E0658E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/A6CE4C2B-8D45-DF11-BB83-003048673F54.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/A61E6DD5-F945-DF11-854F-0025B3E063CA.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/9EACA223-8745-DF11-8795-00E08178C01B.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/96E46086-9045-DF11-957D-0025B3E063E8.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/8015957B-8845-DF11-BEF1-0025B3E05C6E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/7E28D227-8945-DF11-A784-00E08178C0FD.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/6AF1C7A5-9245-DF11-B5FE-003048635B88.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/6ACF7F28-8945-DF11-BD6A-003048635B88.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/665FD02D-8945-DF11-9D49-002481E15104.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/6423CA1F-8B45-DF11-9937-0025B3E0658E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/5C92282C-8745-DF11-ACD8-00E08178C0FD.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/5A2B77CD-8745-DF11-A062-003048D479FC.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/52EB567B-8845-DF11-8925-00E08178C097.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/524E965B-4B46-DF11-826F-0025B3E065DC.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/4E58C4E6-4F46-DF11-BEC2-0025B3E05CDA.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/4C0B91C5-8F45-DF11-A1F7-00E08178C027.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/4AF55D92-8645-DF11-944E-002481E14E62.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/3241021A-8B45-DF11-8EC1-002481E15104.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/3002AAE6-8945-DF11-ACA1-003048635B86.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/2CD84CD6-8945-DF11-A1A8-003048635E0E.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/2A065180-F045-DF11-A4CF-0025B3E06468.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/284C15D9-2046-DF11-9B65-0025B3E05DB6.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/1C3E9E7B-8845-DF11-9235-00E08178C18D.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/18CAF329-8945-DF11-A782-003048673F54.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/14A764CD-8745-DF11-9A8F-003048D45F3A.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/10AC96DB-8146-DF11-BE47-003048670A0C.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/0E4E21FE-8545-DF11-BFF0-0025B3E063A0.root'
, '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/961/0C201342-8A45-DF11-A6B4-00E08178C027.root'
))