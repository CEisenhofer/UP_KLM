(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1)) v2))
(assert (edge v0 (or v0 (not v3))))
(assert (edge true (or v2 v4 (not v0))))
(assert (edge (not v4) (or v3 v4)))
(assert (edge (or (not v1) v2 v3) (or (not v2) (not v4))))
(assert (edge v3 (or (not v1) (not v4))))
(assert (edge (or v0 (not v2)) (or v2 (not v4))))
(assert (edge (or v3 v4) true))
(assert (edge (or v3 (not v0)) (or (not v1) (not v0))))
(assert (edge (or v0 (not v1) v4) true))
(assert (edge (or (not v0) (not v3)) (or v2 (not v4) (not v0))))
(assert (edge (not v1) (or (not v0) (not v1))))
(assert (edge (or v0 v1) (not v1)))
(assert (edge (or v0 (not v3)) (or v1 (not v3))))
(assert (edge (not v3) true))
(assert (edge (or v0 (not v4)) (or v0 v4)))
(assert (edge true (or v3 v4 (not v2))))
(assert (edge v4 (or v0 v1 (not v2))))
(assert (edge (or (not v1) (not v3)) (not v3)))
(assert (edge (or v0 v3) (or v0 v1 (not v2))))
(assert (not (edge v1 (or v1 v3))))