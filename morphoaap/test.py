import numpy as np
import morphoaap as aap

img = np.array([
    [5,5,5],
    [3,8,8],
    [2,3,8]
])

quant_linhas, quant_colunas = img.shape
img_vetor = img.ravel()

tree = aap.ComponentTree(img_vetor, quant_linhas, quant_colunas, False)

def percursoProfundidade(node):
  print("rep:"+str(node.rep), "level:"+str(node.level))
  for child in node.children:
    percursoProfundidade(child)

print("Vetor parent:", tree.parent)
print("Percurso em profundidade")
percursoProfundidade(tree.root)


attr_area = aap.Attribute.computerArea(tree)
filter = aap.AttributeFilters(tree)
img_filtered = filter.prunningMin(attr_area, 5)
print("Imagem filtrada:")
print( img_filtered )

img_filtered2 = filter.prunningMinByAdaptativeThreshold(attr_area, 5, 1)
print("Imagem filtrada (adap):")
print( img_filtered2 )


ap = aap.AttributeProfile(img_vetor, quant_linhas, quant_colunas)
attr_type = 0 #AREA
thresholds = [3, 5]
attr_profile = ap.getAP(thresholds, attr_type);
print("attribute profile")
print(attr_profile)

adap_attr_profile = ap.getAAP(thresholds, attr_type, 1);
print("adaptive attribute profile")
print(adap_attr_profile)