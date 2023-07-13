import numpy as np
import aap

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
img_filtered = aap.AttributeFilters.prunningMin(tree, attr_area, 5)
print("Imagem filtrada:")
print( img_filtered )

ap = aap.AttributeProfile(img_vetor, quant_linhas, quant_colunas)
attr_type = 0 #AREA
thresholds = [3, 5]
attr_profile = ap.getAP(thresholds, attr_type);
print("attribute profile")
print(attr_profile)