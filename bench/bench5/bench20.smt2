(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1) v2) (or v2 (not v3) (not v0))))
(assert (edge (or v1 (not v0) (not v2)) (or v0 (not v4))))
(assert (edge (or v2 (not v4)) v3))
(assert (edge v0 (or v3 (not v0))))
(assert (edge true true))
(assert (not (edge v2 true)))