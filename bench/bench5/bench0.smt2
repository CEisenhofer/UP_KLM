(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1)) v0))
(assert (edge (or v2 v3) (or (not v1) (not v3) v4)))
(assert (edge (or v0 v1 (not v4)) (or v0 (not v1) (not v3))))
(assert (edge (or v2 (not v4) (not v0)) (or v1 v3)))
(assert (edge true (not v1)))
(assert (not (edge (not v1) (or v0 v4))))